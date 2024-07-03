#include "GameScene.h"
#include "AxisIndicator.h"
#include "Function.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
	delete enemy_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	// 敵キャラの生成
	enemy_ = new Enemy();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	// 敵キャラの初期化
	enemy_->Initialize(model_, {0.0f, 4.0f, 140.0f});
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();
	// 敵キャラの更新
	enemy_->Update();

	// 当たり判定
	CheckAllCollision();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_TAB)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}
#endif // _DEBUG
	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);
	// 敵キャラの描画
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	// コライダー
	std::list<Collider*> colliders_;
	// コライダーをリストに登録
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);
	//自弾全てについて
	for (PlayerBullet* pbullet : playerBullets) {
		colliders_.push_back(pbullet);
	}
	//敵弾全てについて
	for (EnemyBullet* ebullet : enemyBullets) {
		colliders_.push_back(ebullet);
	}

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;
		// イテレータBはイテレーターAの次の要素から回す
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			// イテレーターBからコライダーBを取得する
			Collider* colliderB = *itrB;
			// 衝突フィルタリング
			if (
				!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
				!(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())
				) {
				continue;
			}
			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA, posB;
	// Aの座標
	posA = colliderA->GetWorldPosition();
	// Bの座標
	posB = colliderB->GetWorldPosition();
	// 座標AとBの距離を求める
	float length = Length(posA, posB);
	// 球と球の交差判定
	if (length <= (colliderA->GetRadius() + colliderB->GetRadius())) {
		// 衝突時コールバックを呼び出す
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}

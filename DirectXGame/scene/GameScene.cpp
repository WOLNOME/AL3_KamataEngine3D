#include "GameScene.h"
#include "AxisIndicator.h"
#include "Function.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete model_;
	delete modelSkydome_;
	delete debugCamera_;
	delete railCamera_;
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	// 天球の生成
	skydome_ = new Skydome();
	// 自キャラの初期化
	Vector3 playerPosition(0, 0, 20.0f);
	player_->Initialize(model_, textureHandle_, playerPosition);
	//自キャラにゲームシーンを渡す
	player_->SetGameScene(this);
	//敵キャラにゲームシーンを渡す
	enemy_->SetGameScene(this);

	// 天球の初期化
	skydome_->Initialize(modelSkydome_, {0.0f, 0.0f, 0.0f});

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// レールカメラの生成
	railCamera_ = new RailCamera();
	// レールカメラの初期化
	railCamera_->Initialize({0.0f, 0.0f, -6.0f}, {0.0f, 0.0f, 0.0f});
	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// 自キャラ(子)とレールカメラ(親)の親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());
}

void GameScene::Update() {
	// デスフラグの立った自弾を削除
	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	//  デスフラグの立った敵弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	//敵の出現処理
	enemyAppearTimer_--;
	if (enemyAppearTimer_ == 0) {
		//タイマーが0になったら敵出現
		// 生成、初期化、セッターの登録
		Enemy* enemy = new Enemy();
		enemy->Initialize(model_, GetWorldPosition());

		// 弾を登録する
		gameScene_->AddPlayerBullet(newBullet);
		//タイマーリセット
		enemyAppearTimer_ = kEnemyAppearInterval;
	}

	// 自キャラの更新
	player_->Update();
	// 敵キャラの更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	// 自弾更新
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}
	// 敵弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	// 天球の更新
	skydome_->Update();

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
	/// カメラの処理

	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

	} else {
		// レールカメラの更新
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
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
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}
	// 自弾描画
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(viewProjection_);
	}
	// 敵弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
	// 天球の描画
	skydome_->Draw(viewProjection_);

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
	// 衝突判定AとBの座標
	Vector3 posA, posB;

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		float length = Length(posA, posB);
		// 球と球の交差判定
		if (length <= pow(player_->GetRadius() + bullet->GetRadius(), 2)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突字コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();

	// 自弾と敵キャラ全ての当たり判定
	for (PlayerBullet* bullet : playerBullets_) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		float length = Length(posA, posB);
		// 球と球の交差判定
		if (length <= pow(bullet->GetRadius() + enemy_->GetRadius(), 2)) {
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
			// 敵キャラの衝突字コールバックを呼び出す
			enemy_->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自弾と敵キャラ全ての当たり判定
	for (PlayerBullet* pbullet : playerBullets_) {
		for (EnemyBullet* ebullet : enemyBullets_) {
			// 自弾の座標
			posA = pbullet->GetWorldPosition();
			// 敵弾の座標
			posB = ebullet->GetWorldPosition();
			// 座標AとBの距離を求める
			float length = Length(posA, posB);
			// 球と球の交差判定
			if (length <= pow(pbullet->GetRadius() + ebullet->GetRadius(), 2)) {
				// 自弾の衝突時コールバックを呼び出す
				pbullet->OnCollision();
				// 敵キャラの衝突字コールバックを呼び出す
				ebullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	// リストに登録する
	playerBullets_.push_back(playerBullet);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

///敵を生成したい時
//生成
//初期化
//ゲームシーン渡す
//自キャラのアドレス渡す

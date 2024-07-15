#include "Player.h"
#include "Function.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

Player::~Player() { delete sprite2DReticle_; }

void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("target.png");
	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {640.0f, 360.0f}, Vector4(1, 1, 1, 1), {0.5f, 0.5f});

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update(ViewProjection& viewProjection) {

	/// 旋回処理
	// 回転速さ
	const float kRotSpeed = 0.02f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.3f;

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 座標移動（ベクトルの加算）
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	// 移動限界座標
	const float kMoveLimitX = 33.0f;
	const float kMoveLimitY = 17.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	/// 自機のワールド座標から3Dレティクルのワールド座標を計算
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// 自機のワールド行列の回転を反映(offsetは自機の座標由来なので、Normalを使う←原点由来なら通常のTransform)
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = Add(worldTransform_.translation_, offset);

	/// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 worldPositionReticle3D = worldTransform3DReticle_.translation_;
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	Vector3 screenPositionReticle3D = Transform(worldPositionReticle3D, matViewProjectionViewport);
	// 2Dレティクルの座標
	Vector2 positionReticle2D = {screenPositionReticle3D.x, screenPositionReticle3D.y};
	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(positionReticle2D);

	// ロックオン処理
	LockOnProcess2(positionReticle2D, screenPositionReticle3D, matViewProjectionViewport);

	// 攻撃処理
	Attack(worldPositionReticle3D);

	// 3Dレティクル行列の更新
	worldTransform3DReticle_.UpdateMatrix();
	// 行列の更新
	worldTransform_.UpdateMatrix();

	// キャラクターの座標を画面表示する処理
	//  デバッグテキストの表示
	ImGui::Begin(" ");
	// float3スライダー

	inputFloat3[0] = worldTransform_.translation_.x;
	inputFloat3[1] = worldTransform_.translation_.y;
	inputFloat3[2] = worldTransform_.translation_.z;

	ImGui::SliderFloat3("player", inputFloat3, -40.0f, 40.0f);

	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void Player::DrawUI() {
	// 2Dスプライト張る用
	sprite2DReticle_->Draw();
	// ロックオン2Dスプライト
	for (size_t i = 0; i < kEnemyNum; ++i) {
		if (lockOnReticle2D_.at(i).isLockOn) {
			spriteLockOnReticle2D_.at(i)->Draw();
		}
	}
}

void Player::Attack(const Vector3& positionReticle3D) {
	// ロックオン判定
	bool isLockOn = false;
	for (size_t i = 0; i < lockOnReticle2D_.size(); ++i) {
		if (lockOnReticle2D_.at(i).isLockOn) {
			isLockOn = true;
			continue;
		}
	}
	// ロックオンされてる場合
	if (isLockOn) {
		// 弾の数分
		for (size_t i = 0; i < lockOnReticle2D_.size(); ++i) {
			// 弾発射
			if (input_->TriggerKey(DIK_SPACE)) {
				// ロックオンされているポイントに絞る
				if (lockOnReticle2D_.at(i).isLockOn) {

					// 弾の速度
					Vector3 velocity(0, 0, kBulletSpeed);

					// 速度ベクトルを自機の向きに合わせて回転させる
					velocity = Subtract(enemies_.at(i)->GetWorldPosition(), worldTransform_.translation_);
					velocity = Multiply(kBulletSpeed, Normalize(velocity));

					// 弾を生成し、初期化
					PlayerBullet* newBullet = new PlayerBullet();
					newBullet->Initialize(model_, GetWorldPosition(), velocity);

					// 弾を登録する
					gameScene_->AddPlayerBullet(newBullet);
				}
			}
		}
	}
	// ロックオンされてない場合
	else {
		// 弾発射
		if (input_->TriggerKey(DIK_SPACE)) {

			// 弾の速度
			Vector3 velocity(0, 0, kBulletSpeed);

			// 速度ベクトルを自機の向きに合わせて回転させる

			velocity = Subtract(positionReticle3D, worldTransform_.translation_);

			velocity = Multiply(kBulletSpeed, Normalize(velocity));

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);

			// 弾を登録する
			gameScene_->AddPlayerBullet(newBullet);
		}
	}
}

void Player::OnCollision() {}

void Player::LockOnProcess2(const Vector2& positionReticle2D, const Vector3& positionReticle3D, const Matrix4x4& viewProjectionViewportMatrix) {
	positionReticle3D;
	/////必要情報の入手/////
	// 敵の情報を入手
	std::list<Enemy*> enemies;
	enemies = gameScene_->GetEnemies();
	// 敵の数
	kEnemyNum = enemies.size();
	// 要素数の再配置←kEnemyNumがenemis.size()由来だから常にサイズは同じ。
	enemies_.resize(kEnemyNum);
	lockOnReticle2D_.resize(kEnemyNum);
	spriteLockOnReticle2D_.resize(kEnemyNum);
	// list→vector
	size_t index = 0;
	for (Enemy* enemy : enemies) {
		enemies_.at(index) = enemy;
		index++;
	}
	index = 0;
	// 敵それぞれのスクリーン座標=ロックオンレティクルの座標を入手
	for (size_t i = 0; i < kEnemyNum; ++i) {
		lockOnReticle2D_.at(i).pos2D.x = Transform(enemies_.at(i)->GetWorldPosition(), viewProjectionViewportMatrix).x;
		lockOnReticle2D_.at(i).pos2D.y = Transform(enemies_.at(i)->GetWorldPosition(), viewProjectionViewportMatrix).y;
	}
	/////ロックオン変数を求める（演算）/////
	// デフォルトの2Dレティクルとロックオンレティクルの距離を比較。→もし規定範囲内なら、フラグをtrueにする
	for (size_t i = 0; i < kEnemyNum; ++i) {
		if (Length(positionReticle2D, lockOnReticle2D_.at(i).pos2D) < kLockOnStrength) {
			lockOnReticle2D_.at(i).isLockOn = true;
		}
	}
	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("target.png");
	// スプライト設定
	for (size_t i = 0; i < kEnemyNum; i++) {
		// もし生成されてないなら
		if (spriteLockOnReticle2D_.at(i) == NULL) {
			// 生成
			spriteLockOnReticle2D_.at(i) = Sprite::Create(textureReticle, {640.0f, 360.0f}, Vector4(1, 0, 0, 1), {0.5f, 0.5f});
		}
		// 座標設定
		spriteLockOnReticle2D_.at(i)->SetPosition(lockOnReticle2D_.at(i).pos2D);
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

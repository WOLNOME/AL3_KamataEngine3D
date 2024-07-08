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
	Vector3 positionReticle3D = worldTransform3DReticle_.translation_;
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle3D = Transform(positionReticle3D, matViewProjectionViewport);
	Vector2 positionReticle2D = {positionReticle3D.x, positionReticle3D.y};
	//ロックオン処理
	LockOnProcess(positionReticle2D, positionReticle3D, matViewProjectionViewport);
	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(positionReticle2D);

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
}

void Player::Attack(const Vector3& lockOnEnemyPos) {
	// 弾発射
	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		if (!isLockOn) {
			//ロックオンしていないなら
			velocity = Subtract(worldTransform3DReticle_.translation_, worldTransform_.translation_);
		} 
		else {
			//ロックオンしていたら
			velocity = Subtract(lockOnEnemyPos, worldTransform_.translation_);
		}
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		gameScene_->AddPlayerBullet(newBullet);
	}
}

void Player::OnCollision() {}

void Player::LockOnProcess(Vector2& positionReticle2D, const Vector3& positionReticle3D, const Matrix4x4& viewProjectionViewportMatrix) {
	//ラープ中は以下の処理を行わない
	if(!isLerp) {
		// 敵のワールド座標を得る
		std::list<Enemy*> enemies;
	    // もしenemyが空ならreturn
	    if (enemies.size() == 0) {
		    return;
	    }
		std::vector<Vector3> worldEnemyPos;
		size_t i = 0;
		for (Enemy* enemy : enemies) {
			worldEnemyPos.at(i) = enemy->GetWorldPosition();
			i++;
		}
		i = 0;
		// 得たワールド座標をスクリーン座標に変換する
		std::vector<Vector3> screenEnemyPos3D;
		std::vector<Vector2> screenEnemyPos2D;
		for (Vector3 wep : worldEnemyPos) {
			screenEnemyPos3D.at(i) = Transform(wep, viewProjectionViewportMatrix);
			screenEnemyPos2D.at(i) = {screenEnemyPos3D.at(i).x, screenEnemyPos3D.at(i).y};
			i++;
		}
		i = 0;
		// 2Dレティクルの位置と敵のスクリーン座標の位置から距離をそれぞれ求める
		std::vector<float> lengths;
		for (Vector2 sep : screenEnemyPos2D) {
			//参照している値がpositionReticle2D→ロックオンしたら常に同じ敵を追い続ける
			lengths.at(i) = Length(sep, positionReticle2D);
			i++;
		}
		i = 0;
		// 求めたlengthから最も距離の短いlengthのindexを求める
		size_t minIndex = std::distance(lengths.begin(), std::min_element(lengths.begin(), lengths.end()));
		// 求めたindexに当てはまるlengthをロックオンの範囲と計算
		if (lengths.at(minIndex) < kLockOnStrength) {
			// ロックオン！
			isLockOn = true;
		} else {
			// ロックオンしない
			isLockOn = false;
		}
		// もしロックオン成功したら
		if (isLockOn) {
			// 2Dレティクルを敵の座標に更新
			positionReticle2D = screenEnemyPos2D.at(minIndex);
			// 色を変更
			sprite2DReticle_->SetColor(Vector4(1.0f, 0.0f, 0.0, 1.0f));

			// 最後にisPreLockOnの更新
			isPreLockOn = true;

		} else {
			// 色をそのままor元に戻す
			sprite2DReticle_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			// 前フレームでロックオン状態なら
			if (isPreLockOn) {
				// ラープフラグオン
				isLerp = true;
				// ラープスタート地点
				positionResticleBefore2D = positionReticle2D;
				// ラープゴール地点
				positionResticleAfter2D = {positionReticle3D.x, positionReticle3D.y};
			}

			// 最後にisPreLockOnの更新
			isPreLockOn = false;
		}
		// 攻撃処理
		Attack(worldEnemyPos.at(minIndex));
	}
	// ラープ処理
	if (isLerp) {
		positionReticle2D = Lerp(positionResticleBefore2D, positionResticleAfter2D, (float)t);
		t++;
		if (t >= kBackTime) {
			t = 0;
			isLerp = false;
		}
		// 攻撃処理
		Attack(Vector3(0.0f, 0.0f, 0.0f));
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

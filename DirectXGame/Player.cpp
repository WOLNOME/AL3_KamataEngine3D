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
	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_W)) {
		worldTransform_.rotation_.x -= kRotSpeed;
	} else if (input_->PushKey(DIK_S)) {
		worldTransform_.rotation_.x += kRotSpeed;
	}
	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

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

	// 攻撃処理
	Attack();

	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	//マウスの位置にスプライトを置く
	ScreenToWorldMouse(matViewport, viewProjection);

	// 行列の更新
	worldTransform_.UpdateMatrix();

	// キャラクターの座標を画面表示する処理
	//  デバッグテキストの表示
	ImGui::Begin("player ");
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

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Player::DrawUI() {
	//2Dスプライト張る用
	sprite2DReticle_->Draw();
}

void Player::Attack() {
	// 弾発射
	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = Subtract(worldTransform3DReticle_.translation_, worldTransform_.translation_);
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		gameScene_->AddPlayerBullet(newBullet);
	}
}

void Player::OnCollision() {}

void Player::ScreenToWorldMouse(Matrix4x4 matViewport, ViewProjection& viewProjection) {
	//マウス座標を取得
	POINT mousePosition;
	GetCursorPos(&mousePosition);

	//クライアントエリア座標(スクリーン座標)に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	//マウス座標を2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition(Vector2((float)mousePosition.x, (float)mousePosition.y));
	
	//ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear = Vector3((float)mousePosition.x, (float)mousePosition.y, 0);
	Vector3 posFar = Vector3((float)mousePosition.x, (float)mousePosition.y, 1);

	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);
	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 50.0f;
	worldTransform3DReticle_.translation_ = Add(posNear, Multiply(kDistanceTestObject, mouseDirection));
	//ワールド行列の更新と転送
	worldTransform3DReticle_.UpdateMatrix();

	//デバッグ文字表示
	ImGui::Begin("player");
	ImGui::Text("2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();

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

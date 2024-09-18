#include "Player.h"
#include "Function.h"
#include <cassert>

void Player::Initialize(Model* model, const Vector3& position, Input* input) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// インプットセット
	input_ = input;
}

void Player::Update() {
	// 移動処理
	XINPUT_STATE padInput;
	if (input_->GetJoystickState(0, padInput)) {
		// しきい値設定
		const float threshold = 0.7f;
		bool isMoving = false;
		// 速さ
		const float speed = 0.3f;
		// 移動量
		Vector3 move = Normalize({(float)padInput.Gamepad.sThumbLX, 0.0f, (float)padInput.Gamepad.sThumbLY});
		if (!padInput.Gamepad.sThumbLX && !padInput.Gamepad.sThumbLY) {
			move = {0.0f, 0.0f, 0.0f};
		}
		if (Length(move) > threshold) {
			isMoving = true;
		}
		// 移動量に速さを反映
		move = Multiply(speed, move);
		// 移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		move = TransformNormal(move, rotateMatrix);
		if (isMoving) {
			// 移動処理
			worldTransform_.translation_ = Add(worldTransform_.translation_, move);
			// 目標角度の算出
			goalRadian = std::atan2f(move.x, move.z);
		}
	}
	//最短角度補間
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, goalRadian, 0.4f);

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

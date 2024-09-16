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
	input_->GetJoystickState(0, padInput);
	// 速さ
	const float speed = 0.3f;
	// 移動量
	Vector3 move = {(float)padInput.Gamepad.sThumbLX, 0.0f, (float)padInput.Gamepad.sThumbLY};
	// 移動量に速さを反映
	move = Multiply(speed, Normalize(move));
	if (!padInput.Gamepad.sThumbLX && !padInput.Gamepad.sThumbLY) {
		move = {0.0f, 0.0f, 0.0f};
	}
	
	//移動ベクトルをカメラの角度だけ回転する
	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
	move = TransformNormal(move, rotateMatrix);
	
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	//移動方向きを合わせる
	worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(move, rotateYMatrix);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

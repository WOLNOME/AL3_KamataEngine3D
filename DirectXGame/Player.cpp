#include "Player.h"
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
	if (input_->GetJoystickState())

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

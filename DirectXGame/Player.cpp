#include "Player.h"
#include <cassert>

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(Model* model, const Vector3& position)
{
	//NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

}

void Player::Update(char* keys, char* preKeys)
{
	keys, preKeys;

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

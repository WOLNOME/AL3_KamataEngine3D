#include "Enemy.h"
#include <cassert>
#include "TextureManager.h"
#include "Function.h"

void Enemy::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

}

void Enemy::Update() {
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//速度加算
	switch (phase_) {
	case Phase::Approach:
		//移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
		//既定の位置に達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}

		break;
	case Phase::Leave:
		// 移動
		worldTransform_.translation_ = Subtract(worldTransform_.translation_, velocity_);
		// 既定の位置に達したら離脱
		if (worldTransform_.translation_.z > 160.0f) {
			phase_ = Phase::Approach;
		}
		break;
	default:
		break;
	}




}

void Enemy::Draw(ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

#include "Enemy.h"
#include "Function.h"
#include "TextureManager.h"
#include <cassert>

Enemy::Enemy() { }

Enemy::~Enemy() { }

void Enemy::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//初期化状態をセット
	ChangeState(std::make_unique<EnemyStateApproach>(this));

}

void Enemy::Update() {
	// 速度加算
	state_->Update();

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> state) { state_ = std::move(state); }


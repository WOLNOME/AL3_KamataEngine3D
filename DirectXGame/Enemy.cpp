#include "Enemy.h"
#include "Function.h"
#include "ImGuiManager.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(const std::vector<Model*>& models) {
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransformBase_.translation_ = {0.0f, 0.0f, 0.0f};
	// パーツの座標を設定
	worldTransformL_arm_.translation_ = {-1.0f, 1.0f, 0.0f};
	worldTransformR_arm_.translation_ = {1.0f, 1.0f, 0.0f};
	// 親子関係を結び
	worldTransformBase_.parent_ = &worldTransform_;
	worldTransformBody_.parent_ = &worldTransformBase_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
}

void Enemy::Update() {

	UpdateFloatingGimmick();

#ifdef _DEBUG
	ImGui::Begin("enemy");
	ImGui::DragFloat3("body", &worldTransformBody_.translation_.x, 0.01f);
	ImGui::DragFloat3("L_arm", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("R_arm", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::End();
#endif // _DEBUG

	// 基底クラスの更新
	BaseCharacter::Update();
	// ワールドトランスフォームの更新
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	//
	models_[kModelIndexEnemyBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexEnemyL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexEnemyR_arm]->Draw(worldTransformR_arm_, viewProjection);
}

void Enemy::UpdateFloatingGimmick() {
	// 回転させる
	worldTransformBase_.rotation_.y += (2 * pi) / kIntervalTime_;
	//回転行列を作る
	Matrix4x4 rotateMatrix = MakeRotateYMatrix(worldTransformBase_.rotation_.y);
	Vector3 velocity = {0.0f, 0.0f, kSpeed_};
	velocity = TransformNormal(velocity, rotateMatrix);
	//移動
	worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, velocity);
}

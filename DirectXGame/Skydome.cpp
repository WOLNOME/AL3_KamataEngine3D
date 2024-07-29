#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewProjection) {
	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

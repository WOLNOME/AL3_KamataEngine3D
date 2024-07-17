#include "FPSCamera.h"
#include "Function.h"
#include "Player.h"
#include <imgui.h>

void FPSCamera::Initialize(const Vector3& position, const Vector3& radian) {
	// ワールドトランスフォームの初期設定
	worldTransForm_.translation_ = position;
	worldTransForm_.rotation_ = radian;
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}

void FPSCamera::Update() {
	// 移動の加算
	worldTransForm_.translation_ = player_->GetLocalPosition();

	// 回転の加算
	worldTransForm_.rotation_ = player_->GetLocalRotation();

	// ワールドトランスフォームのワールド行列再計算
	worldTransForm_.matWorld_ = MakeAffineMatrix(worldTransForm_.scale_, worldTransForm_.rotation_, worldTransForm_.translation_);

	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransForm_.matWorld_);

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &worldTransForm_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &worldTransForm_.rotation_.x, 0.01f);
	ImGui::End();
}
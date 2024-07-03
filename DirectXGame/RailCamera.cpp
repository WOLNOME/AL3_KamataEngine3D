#include "RailCamera.h"
#include "Function.h"
#include <imgui.h>

void RailCamera::Initialize(const Vector3& position, const Vector3& radian) {
	// ワールドトランスフォームの初期設定
	worldTransForm_.translation_ = position;
	worldTransForm_.rotation_ = radian;
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	// スプライン曲線の制御点(通過点)
	controlPoints_ = {
	    {0,  0,  0},
        {10, 10, 0},
        {10, 15, 0},
        {20, 15, 0},
        {20, 0,  0},
        {30, 0,  0}
    };
	// 線分の数
	const size_t segmentCount = 100;
	// 線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = CatmullRom(controlPoints_, t);
		// 描画用頂点リストに追加
		pointsDrawing.push_back(pos);
	}
}

void RailCamera::Update() {
	// 移動の加算
	worldTransForm_.translation_.z += -0.5f;

	// 回転の加算

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

void RailCamera::Draw() {
	// 曲線描画
	std::vector<Vector3>::iterator itrA = pointsDrawing.begin();
	for (; itrA != pointsDrawing.end(); ++itrA) {
		Vector3 pointA = *itrA;
		std::vector<Vector3>::iterator itrB = itrA;
		itrB++;
		Vector3 pointB = *itrB;
		//ライン描画
		
	}
}

Vector3 RailCamera::CatmullRom(const std::vector<Vector3> controlPoints, const float t) {
	Vector3 result = {0, 0, 0};
	for (size_t i = 0; i < controlPoints.size(); i++) {
		result = Add(result, Multiply(powf(t, float((controlPoints.size() - 1) - i)), controlPoints.at(i)));
	}
	return result;
}

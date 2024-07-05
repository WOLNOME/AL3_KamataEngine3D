#include "RailCamera.h"
#include "DirectXCommon.h"
#include "Function.h"
#include "PrimitiveDrawer.h"
#include <algorithm>
#include <imgui.h>

void RailCamera::Initialize(const Vector3& position, const Vector3& radian) {
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = radian;
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	// スプライン曲線の制御点(通過点)
	controlPoints_ = {
	    {0,  0,  0 },
        {10, 10, 5 },
        {10, 15, 10},
        {20, 15, 15},
        {20, 10, 20},
        {15, 0,  15},
        {10, -5, 5},
        {0,  0,  0 }
    };
	// 線分の数
	const size_t segmentCount = 200;
	// 線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = CatmullRomPosition(controlPoints_, t);
		// 描画用頂点リストに追加
		pointsDrawing.push_back(pos);
	}
}

void RailCamera::Update() {
	/// レールに沿った移動の計算
	// フレームタイマーの計算
	ft++;
	if (ft > loopCount) {
		ft = 0;
	}
	// 視点を求める
	float t1 = 1.0f / loopCount * ft;
	eye = CatmullRomPosition(controlPoints_, t1);
	// 注視点(5f後の点)を求める
	float t2 = 1.0f / loopCount * (ft + 5);
	if (ft + 5 > loopCount) {
		// 5f後が存在しなかったら最初の点に戻る。
		t2 = 1.0f / loopCount * ((ft + 5) - loopCount);
	}
	target = CatmullRomPosition(controlPoints_, t2);

	// 移動の加算(eyeを代入)
	worldTransform_.translation_ = eye;

	// 回転の加算
	// targetとeyeの差分ベクトル(forward)を求める
	Vector3 forward;
	forward = Subtract(target, eye);
	// 進行方向に見た目の回転を合わせる
	//   y軸周りの角度(θy)
	worldTransform_.rotation_.y = std::atan2(forward.x, forward.z);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 forwardZ = Transform(forward, rotateYMatrix);
	// x軸周りの角度(θx)
	worldTransform_.rotation_.x = std::atan2(-forwardZ.y, forwardZ.z);

	// ワールドトランスフォームのワールド行列再計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// カメラオブジェクトワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}

void RailCamera::Draw() {
	// 曲線描画
	std::vector<Vector3>::iterator itrA = pointsDrawing.begin();
	for (; itrA != pointsDrawing.end() - 1; ++itrA) {
		Vector3 pointA = *itrA;
		std::vector<Vector3>::iterator itrB = itrA;
		itrB++;
		Vector3 pointB = *itrB;
		// ライン描画
		PrimitiveDrawer::GetInstance()->DrawLine3d(pointA, pointB, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

#include "FollowCamera.h"
#include "Function.h"

FollowCamera::FollowCamera() {}

FollowCamera::~FollowCamera() {}

void FollowCamera::Initialize(Input* input) {
	// 入力
	input_ = input;
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 3000.0f;
	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};
		//カメラの角度から回転行列を計算する
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);
		//オフセットをカメラの回転に合わせて回転させる
		offset = TransformNormal(offset, rotateMatrix);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);

		XINPUT_STATE padInput;
		input_->GetJoystickState(0, padInput);

		// 回転速度
		float rotationSpeed = 0.03f;
		// 回転成分
		float rotationVelocity;
		if (!padInput.Gamepad.sThumbRX) {
			rotationVelocity = 0.0f;
		} else {
			Vector3 move = {0.0f, (float)padInput.Gamepad.sThumbRX, 0.0f};
			rotationVelocity = Multiply(rotationSpeed, Normalize(move)).y;
		}
		// 速度加算
		viewProjection_.rotation_.y += rotationVelocity;
	}
	// ビュー行列の更新
	viewProjection_.UpdateViewMatrix();
}

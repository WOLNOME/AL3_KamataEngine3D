#include "Player.h"
#include "Function.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm, const Vector3& position, Input* input) {
	// NULLポインタチェック
	assert(modelBody);
	assert(modelHead);
	assert(modelL_arm);
	assert(modelR_arm);
	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;
	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransformBase_.translation_ = position;
	//パーツの座標を設定
	worldTransformHead_.translation_ = {0.0f, 2.2f, 0.0f};
	worldTransformL_arm_.translation_ = {-1.0f, 1.5f, 0.0f};
	worldTransformR_arm_.translation_ = {1.0f, 1.5f, 0.0f};
	//親子関係を結び
	worldTransformBody_.parent_ = &worldTransformBase_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	// インプットセット
	input_ = input;
	
	InitializeFloatingGimmick();

}

void Player::Update() {
	// 移動処理
	XINPUT_STATE padInput;
	if (input_->GetJoystickState(0, padInput)) {
		// しきい値設定
		const float threshold = 0.7f;
		bool isMoving = false;
		// 速さ
		const float speed = 0.3f;
		// 移動量
		Vector3 move = Normalize({(float)padInput.Gamepad.sThumbLX, 0.0f, (float)padInput.Gamepad.sThumbLY});
		if (!padInput.Gamepad.sThumbLX && !padInput.Gamepad.sThumbLY) {
			move = {0.0f, 0.0f, 0.0f};
		}
		if (Length(move) > threshold) {
			isMoving = true;
		}
		// 移動量に速さを反映
		move = Multiply(speed, move);
		// 移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		move = TransformNormal(move, rotateMatrix);
		if (isMoving) {
			// 移動処理
			worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, move);
			// 目標角度の算出
			goalRadian = std::atan2f(move.x, move.z);
		}
	}

	UpdateFloatingGimmick();

	//最短角度補間
	worldTransformBase_.rotation_.y = LerpShortAngle(worldTransformBase_.rotation_.y, goalRadian, 0.4f);

#ifdef _DEBUG
	ImGui::DragFloat3("body", &worldTransformBody_.translation_.x, 0.01f);
	ImGui::DragFloat3("head", &worldTransformHead_.translation_.x, 0.01f);
	ImGui::DragFloat3("L_arm", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("R_arm", &worldTransformR_arm_.translation_.x, 0.01f);

#endif // _DEBUG


	// 行列を定数バッファに転送
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelHead_->Draw(worldTransformHead_, viewProjection);
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	//浮遊移動のサイクル<frame>
	const uint16_t interval = 90;
	//1フレームでのパラメーター加算値
	const float step = 2.0f * pi / interval;
	//パラメーターを1ステップ分加算
	floatingParameter_ += step;
	//2πを超えたら0に戻す
	floatingParameter_ = std::fmodf(floatingParameter_, 2.0 * pi);
	//浮遊の振幅<π>
	const float amplitude = 0.2f;
	//浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude;

}

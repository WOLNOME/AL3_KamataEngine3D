#include "Player.h"
#include "Function.h"
#include "ImGuiManager.h"
#include <cassert>

Player::Player(Input* input) {
	// インプットセット
	input_ = input;
}

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformHammer_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransformBase_.translation_ = {0.0f, 0.0f, 0.0f};
	// パーツの座標を設定
	worldTransformHead_.translation_ = {0.0f, 1.52f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.6f, 1.25f, 0.0f};
	worldTransformR_arm_.translation_ = {0.6f, 1.25f, 0.0f};
	worldTransformHammer_.translation_ = {0.0f, 1.0f, 0.0f};
	// 親子関係を結び
	worldTransformBase_.parent_ = &worldTransform_;
	worldTransformBody_.parent_ = &worldTransformBase_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformHammer_.parent_ = &worldTransformBody_;

	InitializeFloatingGimmick();
}

void Player::Update() {
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Player::Behavior::kRoot:
			BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		default:
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
	// 各振る舞いごとの更新処理
	switch (behavior_) {
	case Player::Behavior::kRoot:
		BehaviorRootUpdate();
		break;
	case Player::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	default:
		break;
	}

	// 基底クラスの更新
	BaseCharacter::Update();
	// 行列を定数バッファに転送
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	//
	models_[kModelIndexPlayerBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexPlayerHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexPlayerL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexPlayerR_arm]->Draw(worldTransformR_arm_, viewProjection);
	models_[kModelIndexPlayerHammer]->Draw(worldTransformHammer_, viewProjection);
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	// 浮遊移動のサイクル<frame>
	const uint16_t interval = 90;
	// 1フレームでのパラメーター加算値
	const float step = 2.0f * pi / interval;
	// パラメーターを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmodf(floatingParameter_, 2.0 * pi);
	// 浮遊の振幅<π>
	const float amplitude = 0.2f;
	// 浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude;
}

void Player::BehaviorRootInitialize() {}

void Player::BehaviorRootUpdate() {
	// 移動処理
	XINPUT_STATE padInput;
	XINPUT_STATE prePadInput;
	input_->GetJoystickStatePrevious(0, prePadInput);
	if (input_->GetJoystickState(0, padInput)) {
		// 攻撃
		if (padInput.Gamepad.wButtons == XINPUT_GAMEPAD_A&&prePadInput.Gamepad.wButtons!=XINPUT_GAMEPAD_A) {
			behaviorRequest_ = Behavior::kAttack;
		}
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

	// 最短角度補間
	worldTransformBase_.rotation_.y = LerpShortAngle(worldTransformBase_.rotation_.y, goalRadian, 0.4f);

#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("body", &worldTransformBody_.translation_.x, 0.01f);
	ImGui::DragFloat3("head", &worldTransformHead_.translation_.x, 0.01f);
	ImGui::DragFloat3("L_arm", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("R_arm", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
}

void Player::BehaviorAttackInitialize() {
	// ハンマー降り下ろし時間
	hammerDownTimer_ = 0;
	isHammerDown_ = false;
	// ハンマー停止時間
	hammerStopTimer_ = 0;
	isHammerStop_ = false;
	// ハンマー降り上げ時間
	hammerUpTimer_ = 0;
	isHammerUp_ = false;
}

void Player::BehaviorAttackUpdate() {
	// 始まり
	if (!isHammerDown_ && !isHammerStop_ && !isHammerUp_) {
		isHammerDown_ = true;
	}
	// 降り下ろし
	if (isHammerDown_) {
		// タイマーインクリメント
		hammerDownTimer_++;
		// 処理
		Vector3 goalRotate = {(1.0f / 2.0f) * pi, 0.0f, 0.0f};
		worldTransformHammer_.rotation_.x += goalRotate.x * (1.0f / kHammerDownTime_);
		// 規定時間に達したら
		if (hammerDownTimer_ >= kHammerDownTime_) {
			hammerDownTimer_ = 0;
			isHammerDown_ = false;
			isHammerStop_ = true;

			worldTransformHammer_.rotation_.x = goalRotate.x;
		}
	}
	// ストップ
	if (isHammerStop_) {
		// タイマーインクリメント
		hammerStopTimer_++;
		// 処理

		// 規定時間に達したら
		if (hammerStopTimer_ >= kHammerStopTime_) {
			hammerStopTimer_ = 0;
			isHammerStop_ = false;
			isHammerUp_ = true;
		}
	}
	// 降り上げ
	if (isHammerUp_) {
		// タイマーインクリメント
		hammerUpTimer_++;
		// 処理
		Vector3 goalRotate = {-(1.0f / 2.0f) * pi, 0.0f, 0.0f};
		worldTransformHammer_.rotation_.x += goalRotate.x * (1.0f / kHammerUpTime_);
		// 規定時間に達したら
		if (hammerUpTimer_ >= kHammerUpTime_) {
			hammerUpTimer_ = 0;
			isHammerUp_ = false;
			worldTransformHammer_.rotation_.x = 0.0f;

			behaviorRequest_ = Behavior::kRoot;
		}
	}
}

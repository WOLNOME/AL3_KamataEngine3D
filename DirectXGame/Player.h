#pragma once
#include "BaseCharacter.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "GlobalVariables.h"
#include <optional>

/// <summary>
/// 自キャラ
/// </summary>
class Player : public BaseCharacter {
private:
	enum PlayerParts {
		kModelIndexPlayerBody,
		kModelIndexPlayerHead,
		kModelIndexPlayerL_arm,
		kModelIndexPlayerR_arm,
		kModelIndexPlayerHammer,
	};
	//振る舞い
	enum class Behavior {
		kRoot,//通常状態
		kAttack,//攻撃中
		kJump,//ジャンプ中
	};


public:
	Player(Input* input);
	~Player() override;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	void InitializeFloatingGimmick();

	void UpdateFloatingGimmick();

	void BehaviorRootInitialize();
	void BehaviorRootUpdate();

	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate();

	void BehaviorJumpInitialize();
	void BehaviorJumpUpdate();


private:
	//調整項目の適用
	void ApplyGlobalVariables();

public:
	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

public:
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

private:
	Input* input_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformHammer_;
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// モデル
	
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

private:
	// 目標角度
	float goalRadian = 0.0f;
	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	//振る舞い
	Behavior behavior_ = Behavior::kRoot;
	//次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//ハンマー降り下ろし時間
	const int kHammerDownTime_ = 15;
	int hammerDownTimer_ = 0;
	bool isHammerDown_ = false;
	//ハンマー停止時間
	const int kHammerStopTime_ = 40;
	int hammerStopTimer_ = 0;
	bool isHammerStop_ = false;
	// ハンマー降り上げ時間
	const int kHammerUpTime_ = 30;
	int hammerUpTimer_ = 0;
	bool isHammerUp_ = false;
};

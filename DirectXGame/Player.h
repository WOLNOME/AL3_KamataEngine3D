#pragma once
#include "BaseCharacter.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

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
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// モデル
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

private:
	// 目標角度
	float goalRadian = 0.0f;
	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
};

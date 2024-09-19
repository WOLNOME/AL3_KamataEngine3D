#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm, const Vector3& position, Input* input);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

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
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// モデル
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

private:
	//目標角度
	float goalRadian = 0.0f;
	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
};

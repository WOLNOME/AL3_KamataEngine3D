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
	void Initialize(Model* model, const Vector3& position, Input* input);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

public:
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

public:
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

private:
	Input* input_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

private:
	//目標角度
	float goalRadian = 0.0f;
};

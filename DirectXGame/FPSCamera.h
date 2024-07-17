#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"

class FPSCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& radian);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // ゲッター
	ViewProjection& GetViewProjection() { return viewProjection_; }
	WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// キーボード入力
	Input* input_ = nullptr;
};

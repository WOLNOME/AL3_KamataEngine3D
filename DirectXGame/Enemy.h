#pragma once
#include "BaseEnemyState.h"
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"
#include "Model.h"
#include "WorldTransform.h"

class BaseEnemyState;
// 行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	Enemy();
	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

public:
	void SetTranslation(Vector3 translation) { worldTransform_.translation_ = translation; }

public:
	Vector3 GetTranslation() { return worldTransform_.translation_; }

public:
	void ChangeState(BaseEnemyState* newState);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 状態
	BaseEnemyState* state_;
};

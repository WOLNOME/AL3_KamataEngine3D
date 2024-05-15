#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>
//行動フェーズ
enum class Phase {
	Approach,	//接近する
	Leave,		//離脱する
};

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
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

	// 接近フェーズの更新関数
	void ApproachUpdate();

	// 離脱フェーズの更新関数
	void LeaveUpdate();

	void ApproachInitialize();
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

public:
	//発射時間
	static const int kFireInterval = 60;


private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_ = {0, 0, -1};
	//フェーズ
	Phase phase_ = Phase::Approach;
	// 弾
	std::list<EnemyBullet*> bullets_;
	//発射タイマー
	int32_t fireTimer_ = 0;

	// メンバ関数ポインタのテーブル
	static void (Enemy::*spFuncTable[])();
};

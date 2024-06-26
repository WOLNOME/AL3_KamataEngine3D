#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "TimeCall.h"
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
	/// <summary>
	/// 弾を発射してタイマーをリセットするコールバック関数
	/// </summary>
	void FireReset();
	

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

	// メンバ関数ポインタのテーブル
	static void (Enemy::*spFuncTable[])();

private:
	//発射間隔
	uint32_t fireInterval_;

	//時限発動のリスト
	std::list<TimeCall*> timedCalls_;
};

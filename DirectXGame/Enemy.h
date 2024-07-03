#pragma once
#include "EnemyBullet.h"
#include "Model.h"
#include "WorldTransform.h"
#include <list>

// 自機の前方宣言
class Player;

// 行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

/// <summary>
/// 敵
/// </summary>
class Enemy : public Collider {
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

public: // 関数
	// 接近フェーズの更新関数
	void ApproachUpdate();

	// 離脱フェーズの更新関数
	void LeaveUpdate();

	void ApproachInitialize();
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

public: // セッター
	void SetPlayer(Player* player) { player_ = player; }

public: // ゲッター
	Vector3 GetWorldPosition() override;

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	// 半径
	float GetRadius() { return rad_; };

public:
	// 発射時間
	static const int kFireInterval = 60;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_ = {0, 0, -0.5f};
	// フェーズ
	Phase phase_ = Phase::Approach;
	// 弾
	std::list<EnemyBullet*> bullets_;
	// 弾の速度
	const float bulletSpeed_ = 1.0f;
	// 発射タイマー
	int32_t fireTimer_ = 0;

	// メンバ関数ポインタのテーブル
	static void (Enemy::*spFuncTable[])();

	// 自キャラ
	Player* player_ = nullptr;
	// 半径
	const float rad_ = 1.0f;
};

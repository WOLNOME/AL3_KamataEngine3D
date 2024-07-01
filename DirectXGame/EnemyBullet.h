#pragma once
#include "Collider.h"
#include "Model.h"
#include "Player.h"
#include "WorldTransform.h"

class Player;

class EnemyBullet : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

public: // 関数
	bool isDead() const { return isDead_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

public: // ゲッター
	Vector3 GetWorldPosition() override;
	// 半径
	float GetRadius() { return rad_; };

public: // セッター
	void SetPlayer(Player* player) { player_ = player; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 移動ベクトル
	Vector3 velocity_;
	// スピード
	float speed_ = 1.0f;
	// 寿命
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
	// 半径
	const float rad_ = 1.0f;

	// プレイヤー
	Player* player_ = nullptr;
};

#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>

/// <summary>
/// 自キャラ
/// </summary>

//関数

class Player:public Collider {
public:

	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,uint32_t textureHandle);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

public://関数
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision()override;

public://ゲッター
	//ワールド座標を取得
	Vector3 GetWorldPosition()override;
	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	// 半径
	float GetRadius() { return rad_; };


private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
	//デバッグテキスト
	float inputFloat3[3] = {0,0,0};
	//弾
	std::list<PlayerBullet*> bullets_;
	// 半径
	const float rad_ = 1.0f;

};

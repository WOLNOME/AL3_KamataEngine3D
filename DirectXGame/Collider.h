#pragma once
#include "Vector3.h"

class Collider {
public: // デストラクタ
	virtual ~Collider() = default;

public: // ゲッター
	// 半径を取得
	float GetRadius() { return radius_; };

public: // セッター
	// 半径を設定
	void SetRadius(float& radius) { radius_ = radius; };

public: // 仮想関数
	// 衝突時に呼ばれる関数
	virtual void OnCollision();
	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

private:
	// 衝突半径
	float radius_ = 1.0f;
};

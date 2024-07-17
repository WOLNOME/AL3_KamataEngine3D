#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

// 前方宣言
class Player;

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

public: // セッター
	void SetPlayer(Player* player) { player_ = player; }

public: // ゲッター
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransForm_; }

private:
	// ワールド変換データ
	WorldTransform worldTransForm_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 自キャラ
	Player* player_ = nullptr;
};

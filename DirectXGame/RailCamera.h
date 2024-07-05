#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& radian);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public: // ゲッター
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransForm_; }

private:
	// ワールド変換データ
	WorldTransform worldTransForm_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// スプライン曲線制御点
	std::vector<Vector3> controlPoints_;
	// 線分で描画するようの頂点リスト
	std::vector<Vector3> pointsDrawing;
};

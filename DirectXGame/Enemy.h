#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy : public BaseCharacter {
private:
	enum EnemyParts {
		kModelIndexEnemyBody,
		kModelIndexEnemyL_arm,
		kModelIndexEnemyR_arm,
	};

public:
	Enemy();
	~Enemy() override;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	void UpdateFloatingGimmick();

public:
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

public:

private:
	// ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	
	// カメラのビュープロジェクション
	
	// モデル
	
	// テクスチャハンドル
	
	// カメラのビュープロジェクション
	
	

private:
	//1周の時間
	const int kIntervalTime_ = 60 * 5;
	//前進スピード
	const float kSpeed_ = 0.3f;

};

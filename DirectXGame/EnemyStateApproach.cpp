#include "EnemyStateApproach.h"
#include "Function.h"

void EnemyStateApproach::Update(Enemy* pEnemy) { 
	// 移動
	
	pEnemy_>worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	// 既定の位置に達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

#include "EnemyStateApproach.h"
#include "Function.h"
#include "Enemy.h"

EnemyStateApproach::EnemyStateApproach(Enemy* enemy) : BaseEnemyState("State Approach",enemy){
}

void EnemyStateApproach::Update() { 
	// 移動
	enemy_->SetTranslation(Add(enemy_->GetTranslation(), velocity_));
	// 既定の位置に達したら離脱
	if (enemy_->GetTranslation().z < 0.0f) {
		enemy_->ChangeState(std::make_unique<EnemyStateLeave>(enemy_));
	}
}

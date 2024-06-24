#include "EnemyStateLeave.h"
#include "Function.h"
#include "Enemy.h"


EnemyStateLeave::EnemyStateLeave(Enemy* enemy) : BaseEnemyState("State Approach", enemy) {
}

void EnemyStateLeave::Update() {
	// 移動
	enemy_->SetTranslation(Add(enemy_->GetTranslation(), velocity_));
	// 既定の位置に達したら離脱
	if (enemy_->GetTranslation().z > 240.0f) {
		enemy_->ChangeState(std::make_unique<EnemyStateApproach>(enemy_));
	}
}

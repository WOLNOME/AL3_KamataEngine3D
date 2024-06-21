#include "EnemyStateLeave.h"
#include "Function.h"

void EnemyStateLeave::Update() {
	// 移動
	enemy_->SetTranslation(Add(enemy_->GetTranslation(), velocity_));
	// 既定の位置に達したら離脱
	if (enemy_->GetTranslation().z < 0.0f) {
		enemy_->ChangeState(new EnemyStateApproach());
	}
}

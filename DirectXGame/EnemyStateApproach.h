#pragma once
#include "BaseEnemyState.h"
#include "Vector3.h"
class Enemy;
class EnemyStateApproach : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);

private:
	const Vector3 velocity_ = {0.0f, 0.0f, -1.0f};
};
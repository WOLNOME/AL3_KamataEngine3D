#pragma once
#include "Enemy.h"
class BaseEnemyState {
public:
	virtual void Update(Enemy* pEnemy) = 0;
};
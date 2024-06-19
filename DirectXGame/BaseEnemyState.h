#pragma once
#include "Enemy.h"
class Enemy;

class BaseEnemyState {
public:
	virtual void Update(Enemy* pEnemy) = 0;
};
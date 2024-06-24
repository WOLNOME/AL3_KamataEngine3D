#pragma once
#include "BaseEnemyState.h"
#include "Vector3.h"
class Enemy;

class EnemyStateLeave : public BaseEnemyState 
{
public:
	EnemyStateLeave(Enemy* enemy);

	void Update();

private:
	const Vector3 velocity_ = {0.0f, 0.0f, 1.0f};
};
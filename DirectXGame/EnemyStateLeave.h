#pragma once
#include "BaseEnemyState.h"
#include "Vector3.h"
class EnemyStateLeave : public BaseEnemyState {
public:
	void Update() override;

private:
	const Vector3 velocity_ = {0.0f, 0.0f, 1.0f};
};
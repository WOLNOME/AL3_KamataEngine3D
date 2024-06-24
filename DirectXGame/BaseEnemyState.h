#pragma once
#include <string>
class Enemy;

class BaseEnemyState {
public:
	virtual ~BaseEnemyState() = default;

	BaseEnemyState(const std::string& name, Enemy* enemy) : name_(name), enemy_(enemy){};

	virtual void Update() = 0;

protected:
	std::string name_;

	Enemy* enemy_ = nullptr;
};
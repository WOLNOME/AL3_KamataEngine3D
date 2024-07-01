#pragma once
#include "Vector3.h"
class Collider {

public:
	virtual ~Collider() = default;
	float GetRadius() { return radius_; }
	void SetRadius(float& radius) { radius_ = radius; }
	virtual void OnCollision();
	virtual Vector3 GetWorldPosition() = 0;

private:
	float radius_ = 1.0f;
};

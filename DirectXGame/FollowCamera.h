#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"

class FollowCamera {
public:
	FollowCamera();
	~FollowCamera();

	void Initialize(Input* input);
	void Update();

public:
	const ViewProjection& GetViewProjection() { return viewProjection_; }

public:
	void SetTarget(const WorldTransform* target) { target_ = target; }

private:
	Input* input_ = nullptr;
	ViewProjection viewProjection_;
	//追従対象
	const WorldTransform* target_ = nullptr;


};

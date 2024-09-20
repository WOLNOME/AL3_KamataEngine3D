#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include <vector>

class BaseCharacter {
public:
	virtual ~BaseCharacter();

	virtual void Initialize(const std::vector<Model*>& models);
	virtual void Update();
	virtual void Draw(const ViewProjection& viewProjection);

public:
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

protected:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//モデル
	std::vector<Model*> models_;

};

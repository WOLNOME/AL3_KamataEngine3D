#pragma once
#include "Model.h"
#include "Function.h"
#include "WorldTransform.h"

class Player
{
public:
	Player();
	~Player();

	void Initialize(Model* model,const Vector3& position);
	void Update(char* keys, char* preKeys);
	void Draw(ViewProjection &viewProjection);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

};


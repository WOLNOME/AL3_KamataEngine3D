#pragma once
#include "WorldTransform.h"
#include "Model.h"


class Skydome {
public:
	void Initialize();
	void Update();
	void Draw();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Model> model_ = nullptr;

};

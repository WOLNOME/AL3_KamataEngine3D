#pragma once
#include <Model.h>
#include "ViewProjection.h"
#include "BaseScene.h"
#include "Player.h"


class StageScene :	public BaseScene
{
public:
	StageScene();
	~StageScene()override;
public:
	void Init() override;
	void Update() override;
	void Draw() override;
public:
	SCENE GetNextScene()override { return NextScene; }
private:
	//3Dモデル
	std::unique_ptr<Model> modelPlayer_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//自キャラ
	std::unique_ptr<Player> player_ = nullptr;

};


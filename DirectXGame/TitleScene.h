#pragma once
#include "BaseScene.h"
class TitleScene : public BaseScene
{
public:
	TitleScene();
	~TitleScene()override;
public:
	void Init() override;
	void Update() override;
	void Draw() override;
public:
	SCENE GetNextScene()override { return NextScene; }
private:
};


#pragma once
#include "SceneManager.h"


class BaseScene
{
public:
	virtual ~BaseScene() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
public:
	virtual SCENE GetNextScene() = 0;
protected:
	//それぞれのシーンにシーン遷移タイミングと遷移先を任せる
	SCENE NextScene;
};


#pragma once
#include "DirectXCommon.h"

const int kAnimationFrame = 20;

enum SCENE {
	TITLE,
	STAGE,
	RESULT
};

class BaseScene;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void ChangeScene();//シーンを変える関数
	void Update();//現在シーンの更新関数
	void Draw();//現在シーンの描画関数
private:

private:
	DirectXCommon* dxCommon_ = nullptr;

	BaseScene* m_pScene;
	SCENE CurrentScene_;
	SCENE NextScene_;
};


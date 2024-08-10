#include "StageScene.h"

StageScene::StageScene()
{
	NextScene = STAGE;
}

StageScene::~StageScene()
{
}

void StageScene::Init()
{
	//3Dモデルの生成
	modelPlayer_.reset(Model::CreateFromOBJ("player", true));
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = std::make_unique<Player>();
	//自キャラの初期化
	player_->Initialize(modelPlayer_.get(), { 0.0f,0.0f,0.0f });


	//カメラの処理

}

void StageScene::Update()
{
	//自キャラの更新

#ifdef _DEBUG
	

#endif // _DEBUG
}

void StageScene::Draw()
{

}

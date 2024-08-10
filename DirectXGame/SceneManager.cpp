#include "SceneManager.h"


#include "BaseScene.h"
#include "TitleScene.h"
#include "StageScene.h"

SceneManager::SceneManager()
{
	m_pScene = new TitleScene();
	CurrentScene_ = TITLE;
	NextScene_ = TITLE;
}

SceneManager::~SceneManager()
{
	delete m_pScene;
}

void SceneManager::ChangeScene()
{
	//シーン切り替えアニメーションとか作るならこの関数に書く

	//切り替え
	if (NextScene_ != CurrentScene_) {
		//現在のシーンの削除
		if (m_pScene != NULL) {
			delete m_pScene;
		}
		//次のシーンの挿入
		switch (NextScene_) {          //引数のシーン
		case SCENE::TITLE:
			m_pScene = new TitleScene();      //タイトルシーンを現在のシーンにする
			CurrentScene_ = m_pScene->GetNextScene();;
			break;
		case SCENE::STAGE:
			m_pScene = new StageScene();      //タイトルシーンを現在のシーンにする
			CurrentScene_ = m_pScene->GetNextScene();;
			break;
		default:
			break;
		}
	}
}

void SceneManager::Update()
{
	//シーン毎の更新処理
	
	//各シーンからNextSceneの情報を受け取る
	if (NextScene_ != m_pScene->GetNextScene()) {
		NextScene_ = m_pScene->GetNextScene();
	}
	//シーンの切り替え処理
	ChangeScene();

}

void SceneManager::Draw()
{

#ifdef _DEBUG

#endif // _DEBUG
}

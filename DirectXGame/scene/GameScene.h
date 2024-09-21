#pragma once

#include <memory>
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "DebugCamera.h"
#include "AxisIndicator.h"
#include "Ground.h"
#include "FollowCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//3Dモデル
	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Model> modelFighterBody_ = nullptr;
	std::unique_ptr<Model> modelFighterHead_ = nullptr;
	std::unique_ptr<Model> modelFighterL_arm_ = nullptr;
	std::unique_ptr<Model> modelFighterR_arm_ = nullptr;
	std::unique_ptr<Model> modelFighterHammer_ = nullptr;
	std::unique_ptr<Model> modelEnemyBody_ = nullptr;
	std::unique_ptr<Model> modelEnemyL_arm_ = nullptr;
	std::unique_ptr<Model> modelEnemyR_arm_ = nullptr;
	std::unique_ptr<Model> modelSkydome_ = nullptr;
	std::unique_ptr<Model> modelGround_ = nullptr;
	//*ワールドトランスフォームは各オブジェクトが各々持つもの
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//自キャラ
	std::unique_ptr<Player> player_ = nullptr;
	//敵キャラ
	std::unique_ptr<Enemy> enemy_ = nullptr;
	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;
	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

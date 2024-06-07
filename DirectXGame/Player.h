#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include <list>

// 前方宣言
class GameScene;

/// <summary>
/// 自キャラ
/// </summary>

// 関数

class Player {
public:
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);
	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

public: // 関数
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置する関数
	void ScreenToWorldMouse(Matrix4x4 matViewport, ViewProjection& viewProjection);

public: // ゲッター
	// ワールド座標を取得
	Vector3 GetWorldPosition();
	// 半径
	float GetRadius() { return rad_; };

public: // セッター
	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォームをセット</param>
	void SetParent(const WorldTransform* parent);
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	// キーボード入力
	Input* input_ = nullptr;
	// デバッグテキスト
	float inputFloat3[3] = {0, 0, 0};
	// 半径
	const float rad_ = 1.0f;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;
};

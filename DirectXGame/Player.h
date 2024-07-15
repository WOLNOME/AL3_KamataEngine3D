#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include <list>

// 前方宣言
class GameScene;
class Enemy;

//構造体
struct LockOnReticle {
	Vector2 pos2D;
	bool isLockOn;
};

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
	void Attack(const Vector3& positionReticle3D);

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// ロックオンに関する関数
	void LockOnProcess2(const Vector2& positionReticle2D, const Vector3& positionReticle3D, const Matrix4x4& viewProjectionViewportMatrix);

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
	// ロックオンの強さ(値が高いほど吸引力高め)
	const float kLockOnStrength = 30;
	// 前フレームでロックオンしていたかの判定
	bool isPreLockOn = false;
	//最後にロックオンした敵の座標を保存
	Vector2 positionLastLockOnEnemy2D = {0.0f, 0.0f};
	// ラープ関連
	int t = 0;
	Vector2 positionResticleBefore2D = {0.0f, 0.0f};
	Vector2 positionResticleAfter2D = {0.0f, 0.0f};
	bool isLerp = false; // ラープ中か
	// ロックオン外れる際に元に戻る時間(フレーム)
	const int kBackTime = 20;

	//new
	//敵の情報(vector型)
	std::vector<Enemy*> enemies_;
	//敵の数
	size_t kEnemyNum = 0;
	//ロックオンレティクル2D(vector型)
	std::vector<LockOnReticle> lockOnReticle2D_;
	//ロックオンレティクル2Dのスプライト用
	std::vector<Sprite*> spriteLockOnReticle2D_;
	//弾の速度
	const float kBulletSpeed = 3.0f;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;
};

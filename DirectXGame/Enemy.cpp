#include "Enemy.h"
#include "Function.h"
#include "TextureManager.h"
#include <cassert>
#include "GameScene.h"
#include "Player.h"

Enemy::~Enemy() {
	
}

void Enemy::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 接近フェーズ初期化
	ApproachInitialize();
}

// フェーズの関数テーブル
void (Enemy::*Enemy::spFuncTable[])() = {&Enemy::ApproachUpdate, &Enemy::LeaveUpdate};

void Enemy::Update() {
	

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	// 速度加算
	//  メンバ関数ポインタに入っている関数を呼び出す
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	liveTimer_++;
	
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	
}


Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::ApproachUpdate() {
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	// 既定の位置に達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
	// 発射タイマーカウントダウン
	fireTimer_--;
	// 指定時間に達した場合
	if (fireTimer_ == 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::LeaveUpdate() {
	// 移動
	worldTransform_.translation_ = Subtract(worldTransform_.translation_, velocity_);
	// 既定の位置に達したら離脱
	if (worldTransform_.translation_.z > 160.0f) {
		phase_ = Phase::Approach;
	}
}
void Enemy::ApproachInitialize() {
	// 発射タイマーを初期化
	fireTimer_ = 10;
}

void Enemy::Fire() {
	// 弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//自キャラのワールド座標を取得する
	Vector3 playerWorldPos;
	playerWorldPos.x = player_->GetWorldPosition().x;
	playerWorldPos.y = player_->GetWorldPosition().y;
	playerWorldPos.z = player_->GetWorldPosition().z;
	//敵キャラのワールド座標を取得する
	Vector3 enemyWorldPos;
	enemyWorldPos.x = GetWorldPosition().x;
	enemyWorldPos.y = GetWorldPosition().y;
	enemyWorldPos.z = GetWorldPosition().z;
	//startとendベクトルの正規化
	Vector3 normalize = Normalize(enemyWorldPos, playerWorldPos,bulletSpeed_);
	//速度を更新
	velocity = normalize;

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_,velocity );

	//弾を登録
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::OnCollision() { isDead_ = true; }

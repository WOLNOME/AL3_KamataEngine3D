#include "CollisionManager.h"
#include "Function.h"

void CollisionManager::CheckCollision() {
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;
		// イテレータBはイテレーターAの次の要素から回す
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			// イテレーターBからコライダーBを取得する
			Collider* colliderB = *itrB;
			// 衝突フィルタリング
			if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) || !(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())) {
				continue;
			}
			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA, posB;
	// Aの座標
	posA = colliderA->GetWorldPosition();
	// Bの座標
	posB = colliderB->GetWorldPosition();
	// 座標AとBの距離を求める
	float length = Length(posA, posB);
	// 球と球の交差判定
	if (length <= (colliderA->GetRadius() + colliderB->GetRadius())) {
		// 衝突時コールバックを呼び出す
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}

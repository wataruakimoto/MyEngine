#include "CollisionManager.h"
#include "Collider.h"
#include "MathVector.h"

using namespace MathVector;

void CollisionManager::Initialize() {

	// コライダーを表示する
	isDraw = true;
}

void CollisionManager::Update() {

	// 表示しないなら
	if (!isDraw) {

		// 更新しない
		return;
	}

	// 全てのコライダーについて
	for (Collider* collider : colliders_) {

		// 更新
		collider->Update();
	}
}

void CollisionManager::Draw() {

	// 表示しないなら
	if (!isDraw) {

		// 描画しない
		return;
	}

	// 全てのコライダーについて
	for (Collider* collider : colliders_) {

		// 描画
		collider->Draw();
	}
}

void CollisionManager::Reset() {

	// コライダーのリストをクリア
	colliders_.clear();
}

void CollisionManager::AddCollider(Collider* collider) {

	// コライダーをリストに追加
	colliders_.push_back(collider);
}

void CollisionManager::CheckAllCollisions() {

	// リスト内のペアを総当りする
	
	// イテレータAはリストの先頭から回す
	std::list<Collider*>::iterator itrA = colliders_.begin();

	for (; itrA != colliders_.end(); ++itrA) {

		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++; // 1つ進める

		for (; itrB != colliders_.end(); ++itrB) {

			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	// 中心座標の取得
	Vector3 posA = colliderA->GetCenterPosition();
	Vector3 posB = colliderB->GetCenterPosition();

	// 座標の差分ベクトル
	Vector3 subtract = posB - posA;

	// 2つの座標の距離を求める
	float distance = Length(subtract);

	// 球と球の衝突判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {

		// それぞれの衝突時のコールバック関数を呼び出す
		colliderA->OnCollision(colliderB);
		colliderB->OnCollision(colliderA);
	}
}

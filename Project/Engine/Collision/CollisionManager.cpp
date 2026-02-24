#include "CollisionManager.h"
#include "Collider.h"
#include "MathVector.h"

#include <algorithm>

using namespace Engine;
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

	// 衝突判定の結果
	bool isColliding = false;


	std::visit([&](auto&& shapeA, auto&& shapeB) {

		using TypeA = std::decay_t<decltype(shapeA)>;
		using TypeB = std::decay_t<decltype(shapeB)>;

		// 球と球の衝突判定
		if constexpr (std::is_same_v<TypeA, Sphere> && std::is_same_v<TypeB, Sphere>) {
			isColliding = CheckSphereToSphere(&shapeA, &shapeB);
		}

		// 球と平面の衝突判定
		else if constexpr (std::is_same_v<TypeA, Sphere> && std::is_same_v<TypeB, Plane>) {
			isColliding = CheckSphereToPlane(&shapeA, &shapeB);
		}

		// 平面と球の衝突判定
		else if constexpr (std::is_same_v<TypeA, Plane> && std::is_same_v<TypeB, Sphere>) {
			isColliding = CheckSphereToPlane(&shapeB, &shapeA);
		}

		// 球とAABBの衝突判定
		else if constexpr (std::is_same_v<TypeA, Sphere> && std::is_same_v<TypeB, AABB>) {
			isColliding = CheckSphereToAABB(&shapeA, &shapeB);
		}

		// AABBと球の衝突判定
		else if constexpr (std::is_same_v<TypeA, AABB> && std::is_same_v<TypeB, Sphere>) {
			isColliding = CheckSphereToAABB(&shapeB, &shapeA);
		}

		// 球とOBBの衝突判定
		else if constexpr (std::is_same_v<TypeA, Sphere> && std::is_same_v<TypeB, OBB>) {
			isColliding = CheckSphereToOBB(&shapeA, &shapeB);
		}

		// OBBと球の衝突判定
		else if constexpr (std::is_same_v<TypeA, OBB> && std::is_same_v<TypeB, Sphere>) {
			isColliding = CheckSphereToOBB(&shapeB, &shapeA);
		}

		// AABBとAABBの衝突判定
		else if constexpr (std::is_same_v<TypeA, AABB> && std::is_same_v<TypeB, AABB>) {
			isColliding = CheckAABBToAABB(&shapeA, &shapeB);
		}

		// AABBとOBBの衝突判定
		else if constexpr (std::is_same_v<TypeA, AABB> && std::is_same_v<TypeB, OBB>) {
			isColliding = CheckAABBToOBB(&shapeA, &shapeB);
		}

		// OBBとAABBの衝突判定
		else if constexpr (std::is_same_v<TypeA, OBB> && std::is_same_v<TypeB, AABB>) {
			isColliding = CheckAABBToOBB(&shapeB, &shapeA);
		}

		// OBBとOBBの衝突判定
		else if constexpr (std::is_same_v<TypeA, OBB> && std::is_same_v<TypeB, OBB>) {
			isColliding = CheckOBBToOBB(&shapeA, &shapeB);
		}
		// その他・未実装の組み合わせ
		else {
			// 衝突していないとする
			isColliding = false;
		}

		}, colliderA->GetShape(), colliderB->GetShape());

	// 衝突しているなら
	if (isColliding) {
		// コールバック関数を呼び出す
		colliderA->OnCollision(colliderB);
		colliderB->OnCollision(colliderA);
	}
}

bool CollisionManager::CheckSphereToSphere(const Sphere* sphereA, const Sphere* sphereB) {

	// 中心点の距離を求める
	float distance = Length(sphereB->center - sphereA->center);

	// 距離が半径の和以下なら
	if (distance <= sphereA->radius + sphereB->radius) {

		// 衝突している
		return true;
	}

	// 衝突していない
	return false;
}

bool CollisionManager::CheckSphereToPlane(const Sphere* sphere, const Plane* plane) {

	// 球の中心点から平面への距離を求める
	float distance = Dot(sphere->center, plane->normal) - plane->distance;

	// 距離の絶対値が半径以下なら
	if (abs(distance) <= sphere->radius) {

		// 衝突している
		return true;
	}

	// 衝突していない
	return false;
}

bool CollisionManager::CheckSphereToAABB(const Sphere* sphere, const AABB* aabb) {

	// 最近接点を求める
	Vector3 closestPoint = {
		std::clamp(sphere->center.x, aabb->min.x, aabb->max.x),
		std::clamp(sphere->center.y, aabb->min.y, aabb->max.y),
		std::clamp(sphere->center.z, aabb->min.z, aabb->max.z)
	};

	// 最近接点と球の中心点の距離を求める
	float distance = Length(closestPoint - sphere->center);

	// 距離が半径以下なら
	if (distance <= sphere->radius) {

		// 衝突している
		return true;
	}

	// 衝突していない
	return false;
}

bool CollisionManager::CheckSphereToOBB(const Sphere* sphere, const OBB* obb) {

	// 実装困難なため、後回し

	//TODO: 球とOBBの衝突判定
	return false;
}

bool CollisionManager::CheckAABBToAABB(const AABB* aabbA, const AABB* aabbB) {

	// 2つのAABBが重なっていない場合
	if (aabbA->max.x < aabbB->min.x || aabbA->min.x > aabbB->max.x ||
		aabbA->max.y < aabbB->min.y || aabbA->min.y > aabbB->max.y ||
		aabbA->max.z < aabbB->min.z || aabbA->min.z > aabbB->max.z) {

		// 衝突していない
		return false;
	}

	// 衝突している
	return true;
}

bool CollisionManager::CheckAABBToOBB(const AABB* aabb, const OBB* obb) {

	// 実装困難なため、後回し

	//TODO: AABBとOBBの衝突判定
	return false;
}

bool CollisionManager::CheckOBBToOBB(const OBB* obbA, const OBB* obbB) {

	// 実装困難なため、後回し

	//TODO: OBB同士の衝突判定

	// 衝突していないとする
	return false;
}

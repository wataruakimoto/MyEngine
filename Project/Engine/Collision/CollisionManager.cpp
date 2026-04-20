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

		// 球とカプセルの衝突判定
		else if constexpr (std::is_same_v<TypeA, Sphere> && std::is_same_v<TypeB, Capsule>) {
			isColliding = CheckSphereToCapsule(&shapeA, &shapeB);
		}

		// カプセルと球の衝突判定
		else if constexpr (std::is_same_v<TypeA, Capsule> && std::is_same_v<TypeB, Sphere>) {
			isColliding = CheckSphereToCapsule(&shapeB, &shapeA);
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

		// AABBとカプセルの衝突判定
		else if constexpr (std::is_same_v<TypeA, AABB> && std::is_same_v<TypeB, Capsule>) {
			isColliding = CheckAABBToCapsule(&shapeA, &shapeB);
		}

		// カプセルとAABBの衝突判定
		else if constexpr (std::is_same_v<TypeA, Capsule> && std::is_same_v<TypeB, AABB>) {
			isColliding = CheckAABBToCapsule(&shapeB, &shapeA);
		}

		// OBBとOBBの衝突判定
		else if constexpr (std::is_same_v<TypeA, OBB> && std::is_same_v<TypeB, OBB>) {
			isColliding = CheckOBBToOBB(&shapeA, &shapeB);
		}

		// OBBとカプセルの衝突判定
		else if constexpr (std::is_same_v<TypeA, OBB> && std::is_same_v<TypeB, Capsule>) {
			isColliding = CheckOBBToCapsule(&shapeA, &shapeB);
		}

		// カプセルとOBBの衝突判定
		else if constexpr (std::is_same_v<TypeA, Capsule> && std::is_same_v<TypeB, OBB>) {
			isColliding = CheckOBBToCapsule(&shapeB, &shapeA);
		}

		// カプセルとカプセルの衝突判定
		else if constexpr (std::is_same_v<TypeA, Capsule> && std::is_same_v<TypeB, Capsule>) {
			isColliding = CheckCapsuleToCapsule(&shapeA, &shapeB);
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

	// OBBのローカル空間での球の中心座標を求める
	Vector3 localPoint = sphere->center - obb->center;

	float distX = Dot(localPoint, obb->orientations[0]);
	float distY = Dot(localPoint, obb->orientations[1]);
	float distZ = Dot(localPoint, obb->orientations[2]);

	// OBBの範囲内にクランプして最近接点を求める
	distX = std::clamp(distX, -obb->halfSize.x, obb->halfSize.x);
	distY = std::clamp(distY, -obb->halfSize.y, obb->halfSize.y);
	distZ = std::clamp(distZ, -obb->halfSize.z, obb->halfSize.z);

	Vector3 closestPoint = obb->center +
		obb->orientations[0] * distX +
		obb->orientations[1] * distY +
		obb->orientations[2] * distZ;

	// 最近接点と球の中心点の距離を求める
	float distance = Length(closestPoint - sphere->center);

	// 距離が半径以下なら衝突している
	return distance <= sphere->radius;
}

bool CollisionManager::CheckSphereToCapsule(const Sphere* sphere, const Capsule* capsule) {
	
	// 線分ベクトル
	Vector3 d = capsule->end - capsule->start;
	float lenSq = Dot(d, d);

	float t = 0.0f;
	if (lenSq > 0.0f) {
		t = Dot(sphere->center - capsule->start, d) / lenSq;
		t = std::clamp(t, 0.0f, 1.0f);
	}

	// カプセルの線分上の最近接点
	Vector3 closestPoint = capsule->start + d * t;

	// 距離を求める
	float distance = Length(closestPoint - sphere->center);

	// カプセルの半径と球の半径の和以下なら衝突
	return distance <= (sphere->radius + capsule->radius);
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

	// AABBをOBBとして扱う
	OBB aabb_obb;
	aabb_obb.center = (aabb->min + aabb->max) * 0.5f;
	aabb_obb.halfSize = (aabb->max - aabb->min) * 0.5f;
	aabb_obb.orientations[0] = { 1.0f, 0.0f, 0.0f };
	aabb_obb.orientations[1] = { 0.0f, 1.0f, 0.0f };
	aabb_obb.orientations[2] = { 0.0f, 0.0f, 1.0f };

	return CheckOBBToOBB(&aabb_obb, obb);
}

bool CollisionManager::CheckAABBToCapsule(const AABB* aabb, const Capsule* capsule) {
	
	// AABBをOBBとして扱う
	OBB aabb_obb;
	aabb_obb.center = (aabb->min + aabb->max) * 0.5f;
	aabb_obb.halfSize = (aabb->max - aabb->min) * 0.5f;
	aabb_obb.orientations[0] = { 1.0f, 0.0f, 0.0f };
	aabb_obb.orientations[1] = { 0.0f, 1.0f, 0.0f };
	aabb_obb.orientations[2] = { 0.0f, 0.0f, 1.0f };

	return CheckOBBToCapsule(&aabb_obb, capsule);
}

bool CollisionManager::CheckOBBToOBB(const OBB* obbA, const OBB* obbB) {

	Vector3 d = obbA->center - obbB->center;

	auto TestAxis = [&](const Vector3& axis) {
		float lenSq = Dot(axis, axis);
		if (lenSq < 1e-6f) return true;
		Vector3 n = axis * (1.0f / std::sqrt(lenSq));

		float rA = std::abs(Dot(n, obbA->orientations[0])) * obbA->halfSize.x +
			std::abs(Dot(n, obbA->orientations[1])) * obbA->halfSize.y +
			std::abs(Dot(n, obbA->orientations[2])) * obbA->halfSize.z;

		float rB = std::abs(Dot(n, obbB->orientations[0])) * obbB->halfSize.x +
			std::abs(Dot(n, obbB->orientations[1])) * obbB->halfSize.y +
			std::abs(Dot(n, obbB->orientations[2])) * obbB->halfSize.z;

		return std::abs(Dot(d, n)) <= rA + rB;
		};

	// 各OBBの3軸をテスト
	for (int i = 0; i < 3; ++i) {
		if (!TestAxis(obbA->orientations[i])) return false;
		if (!TestAxis(obbB->orientations[i])) return false;
	}
	// それぞれの軸の外積をテスト
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			Vector3 cross = Cross(obbA->orientations[i], obbB->orientations[j]);
			if (!TestAxis(cross)) return false;
		}
	}

	return true;
}

bool CollisionManager::CheckOBBToCapsule(const OBB* obb, const Capsule* capsule) {
	
	Vector3 capDir = capsule->end - capsule->start;
	float capLen = Length(capDir);
	if (capLen > 1e-4f) {
		capDir = capDir * (1.0f / capLen);
	}

	Vector3 capCenter = (capsule->start + capsule->end) * 0.5f;
	float capHalfLen = capLen * 0.5f;
	Vector3 d = obb->center - capCenter;

	auto TestAxis = [&](const Vector3& axis) {
		float lenSq = Dot(axis, axis);
		if (lenSq < 1e-6f) return true;
		Vector3 n = axis * (1.0f / std::sqrt(lenSq));

		float rOBB = std::abs(Dot(n, obb->orientations[0])) * obb->halfSize.x +
			std::abs(Dot(n, obb->orientations[1])) * obb->halfSize.y +
			std::abs(Dot(n, obb->orientations[2])) * obb->halfSize.z;

		float rCap = std::abs(Dot(n, capDir)) * capHalfLen + capsule->radius;

		return std::abs(Dot(d, n)) <= rOBB + rCap;
		};

	// OBB の各軸
	for (int i = 0; i < 3; ++i) {
		if (!TestAxis(obb->orientations[i])) return false;
	}
	// Capsule の軸
	if (!TestAxis(capDir)) return false;
	// 外積
	for (int i = 0; i < 3; ++i) {
		Vector3 cross = Cross(obb->orientations[i], capDir);
		if (!TestAxis(cross)) return false;
	}

	return true;
}

bool CollisionManager::CheckCapsuleToCapsule(const Capsule* capsuleA, const Capsule* capsuleB) {
	
	Vector3 d1 = capsuleA->end - capsuleA->start;
	Vector3 d2 = capsuleB->end - capsuleB->start;
	Vector3 r = capsuleA->start - capsuleB->start;
	float a = Dot(d1, d1);
	float e = Dot(d2, d2);
	float f = Dot(d2, r);

	float s = 0.0f, t = 0.0f;

	if (a <= 1e-4f && e <= 1e-4f) {
		s = t = 0.0f;
	}
	else if (a <= 1e-4f) {
		s = 0.0f;
		t = std::clamp(f / e, 0.0f, 1.0f);
	}
	else {
		float c = Dot(d1, r);
		if (e <= 1e-4f) {
			t = 0.0f;
			s = std::clamp(-c / a, 0.0f, 1.0f);
		}
		else {
			float b = Dot(d1, d2);
			float denom = a * e - b * b;

			if (denom != 0.0f) {
				s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else {
				s = 0.0f;
			}
			t = (b * s + f) / e;

			if (t < 0.0f) {
				t = 0.0f;
				s = std::clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t > 1.0f) {
				t = 1.0f;
				s = std::clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	// 各線分上の最近接点
	Vector3 c1 = capsuleA->start + d1 * s;
	Vector3 c2 = capsuleB->start + d2 * t;

	float dist = Length(c1 - c2);
	return dist <= (capsuleA->radius + capsuleB->radius);
}

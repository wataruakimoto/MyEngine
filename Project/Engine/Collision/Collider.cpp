#include "Collider.h"

#include <imgui.h>

using namespace Engine;

void Collider::Initialize() {
}

void Collider::Update() {
}

void Collider::Draw() {

	// 将来的にはワイヤーフレーム等で描画する予定
}

void Collider::SetShape(const CollisionShape& shape) {

	// Noneが設定されていたら
	if (std::holds_alternative<None>(shape_)) {
		
		// 形状を設定
		shape_ = shape;
	}
}

void Collider::SetSphere(const Sphere& sphere) {

	// Noneが設定されていたら
	if (std::holds_alternative<None>(shape_)) {

		// 形状を球に設定
		shape_ = sphere;
	}
	// 球が設定されていたら
	else if (std::holds_alternative<Sphere>(shape_)) {

		// 形状を更新
		shape_ = sphere;
	}
	else {

		return;
	}
}

void Collider::SetPlane(const Plane& plane) {

	// Noneが設定されていたら
	if (std::holds_alternative<None>(shape_)) {

		// 形状を平面に設定
		shape_ = plane;
	}
	// 平面が設定されていたら
	else if (std::holds_alternative<Plane>(shape_)) {

		// 形状を更新
		shape_ = plane;
	}
	else {

		return;
	}
}

void Collider::SetAABB(const AABB& aabb) {

	// Noneが設定されていたら
	if (std::holds_alternative<None>(shape_)) {

		// 形状をAABBに設定
		shape_ = aabb;
	}
	// AABBが設定されていたら
	else if (std::holds_alternative<AABB>(shape_)) {

		// 形状を更新
		shape_ = aabb;
	}
	else {

		return;
	}
}

void Collider::SetOBB(const OBB& obb) {

	// Noneが設定されていたら
	if (std::holds_alternative<None>(shape_)) {

		// 形状をOBBに設定
		shape_ = obb;
	}
	// OBBが設定されていたら
	else if (std::holds_alternative<OBB>(shape_)) {

		// 形状を更新
		shape_ = obb;
	}
	else {

		return;
	}
}



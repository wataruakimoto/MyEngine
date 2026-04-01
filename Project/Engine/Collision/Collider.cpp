#define NOMINMAX

#include "Collider.h"
#include "LineManager.h"

#include <algorithm>
#include <imgui.h>

using namespace Engine;

Collider::Collider(const CollisionShape& shape, uint32_t typeID) {

	// 引数をメンバ変数に設定

	shape_ = shape;

	typeID_ = typeID;
}

Collider::~Collider() {
}

void Collider::Initialize() {

	// インスタンスを取得
	lineManager_ = LineManager::GetInstance();

	// ワールド座標変換を初期化
	worldTransform_.Initialize();
}

void Collider::Update() {

	// ワールド座標変換を更新
	worldTransform_.Update();

	// ワールド座標変換から拡縮を取得
	Vector3 scale = worldTransform_.GetWorldScale();

	// ワールド座標変換からワールド座標を取得
	Vector3 worldPosition = worldTransform_.GetWorldPosition();

	// 形状に応じて更新

	// 形状が球なら
	if (std::holds_alternative<Sphere>(shape_)) {

		Sphere& sphere = std::get<Sphere>(shape_);

		// ワールド座標を球の中心座標に設定
		sphere.center = worldPosition;

		// 拡縮の最大値の半分を設定
		sphere.radius = std::max({ scale.x, scale.y, scale.z }) / 2.0f;
	}
	// 形状がAABBなら
	else if (std::holds_alternative<AABB>(shape_)) {

		AABB& aabb = std::get<AABB>(shape_);

		// ワールド座標と拡縮で計算
		aabb.min = worldPosition - scale;
		aabb.max = worldPosition + scale;
	}
}

void Collider::Draw() {

	// 形状に応じて描画
	if (std::holds_alternative<Sphere>(shape_)) {

		const Sphere& sphere = std::get<Sphere>(shape_);

		// 線描画マネージャで球を描画
		lineManager_->DrawSphere(sphere.center, sphere.radius, 4, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
	else if (std::holds_alternative<AABB>(shape_)) {

		const AABB& aabb = std::get<AABB>(shape_);

		// 線描画マネージャでAABBを描画
		lineManager_->DrawAABB(aabb.min, aabb.max, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void Collider::ShowImGui() {

#ifdef USE_IMGUI

	// ツリーで表示
	if (ImGui::TreeNodeEx("コライダー", ImGuiTreeNodeFlags_Framed)) {

		// ワールド座標変換を表示
		worldTransform_.ShowImGui();

		// IDを表示
		ImGui::Text("タイプID: %u", typeID_);
		
		// ツリーを終了
		ImGui::TreePop();
	}

#endif // USE_IMGUI
}

void Collider::OnCollision(Collider* other) {

	// コールバック関数が設定されていたら
	if (onCollisionCallback_) {

		// コールバック関数を呼び出す
		onCollisionCallback_(other);
	}
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



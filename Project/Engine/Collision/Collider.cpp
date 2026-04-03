#define NOMINMAX

#include "Collider.h"
#include "LineManager.h"
#include "MathVector.h"

#include <algorithm>
#include <imgui.h>

using namespace Engine;
using namespace MathVector;

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
	Vector3 worldScale = worldTransform_.GetWorldScale();

	// ワールド座標変換からワールド座標を取得
	Vector3 worldPosition = worldTransform_.GetWorldPosition();

	// ワールド変換からワールド行列を取得
	Matrix4x4 worldMatrix = worldTransform_.GetWorldMatrix();

	/// ========== 形状に応じた更新 ========== ///

	// 球なら
	if (std::holds_alternative<Sphere>(shape_)) {

		Sphere& sphere = std::get<Sphere>(shape_);

		// ワールド座標を球の中心座標に設定
		sphere.center = worldPosition;

		// 拡縮の最大値の半分を設定
		sphere.radius = std::max({ worldScale.x, worldScale.y, worldScale.z }) / 2.0f;
	}
	// AABBなら
	else if (std::holds_alternative<AABB>(shape_)) {

		AABB& aabb = std::get<AABB>(shape_);

		// ワールド座標と拡縮で計算
		aabb.min = worldPosition - worldScale;
		aabb.max = worldPosition + worldScale;
	}
	// OBBなら
	else if (std::holds_alternative<OBB>(shape_)) {

		OBB& obb = std::get<OBB>(shape_);

		// ワールド座標から中心座標を取得
		obb.center = worldPosition;

		obb.orientations[0] = Normalize({ worldMatrix.m[0][0], worldMatrix.m[0][1], worldMatrix.m[0][2] });
		obb.orientations[1] = Normalize({ worldMatrix.m[1][0], worldMatrix.m[1][1], worldMatrix.m[1][2] });
		obb.orientations[2] = Normalize({ worldMatrix.m[2][0], worldMatrix.m[2][1], worldMatrix.m[2][2] });

		// ワールド変換の拡縮から半分にしたものを取得
		Vector3 halfScale = worldScale / 2.0f;
		obb.halfSize = halfScale;
	}
	// 楕円形なら
	else if (std::holds_alternative<Ellipsoid>(shape_)) {

		Ellipsoid& ellipsoid = std::get<Ellipsoid>(shape_);

		// ワールド座標から中心座標を取得
		ellipsoid.center = worldPosition;

		// 拡縮の半分を設定
		ellipsoid.radius = worldScale / 2.0f;
	}
}

void Collider::Draw() {

	/// ========== 形状に応じた描画 ========== ///

	// 球なら
	if (std::holds_alternative<Sphere>(shape_)) {

		const Sphere& sphere = std::get<Sphere>(shape_);

		// 線で球を描画
		lineManager_->DrawSphere(sphere, 8, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
	// AABBなら
	else if (std::holds_alternative<AABB>(shape_)) {

		const AABB& aabb = std::get<AABB>(shape_);

		// 線でAABBを描画
		lineManager_->DrawAABB(aabb, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
	// OBBなら
	else if (std::holds_alternative<OBB>(shape_)) {

		const OBB& obb = std::get<OBB>(shape_);

		// 線でOBBを描画
		lineManager_->DrawOBB(obb, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
	// 楕円形なら
	else if (std::holds_alternative<Ellipsoid>(shape_)) {

		const Ellipsoid& ellipsoid = std::get<Ellipsoid>(shape_);

		// 線で楕円形を描画
		lineManager_->DrawEllipsoid(ellipsoid, 8, { 1.0f, 1.0f, 1.0f, 1.0f });
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


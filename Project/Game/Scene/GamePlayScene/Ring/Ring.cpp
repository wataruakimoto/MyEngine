#include "Ring.h"
#include "Collision/CollisionTypeIDDef.h"

using namespace Engine;

void Ring::Initialize() {

	// ワールド変換初期化
	worldTransform_.Initialize();

	// コライダー生成
	collider_ = std::make_unique<Collider>(
		AABB{},
		static_cast<uint32_t>(CollisionTypeIDDef::kRing)
	);
	// コライダー初期化
	collider_->Initialize();
	// コライダーに衝突時のコールバック関数を設定
	collider_->SetOnCollision([this](Collider* other) { OnCollision(other); });

	// モデルの生成
	model_ = std::make_unique<Model>();
	// モデルの初期化
	model_->Initialize("Ring/Ring.obj");

	// 3Dオブジェクトの生成
	object_ = std::make_unique<Object3d>();
	// 3Dオブジェクトの初期化
	object_->Initialize();
	// モデルを設定
	object_->SetModel(model_.get());
	// ワールド変換のスケールを3Dオブジェクトに設定
	object_->GetWorldTransform().SetParent(&worldTransform_);
}

void Ring::Update() {

	// ワールド変換の更新
	worldTransform_.Update();

	// コライダーの更新
	collider_->Update();

	// 3Dオブジェクトの更新
	object_->Update();
}

void Ring::Draw() {

	// 3Dオブジェクトの描画
	object_->Draw();
}

void Ring::OnCollision(Collider* other) {
}

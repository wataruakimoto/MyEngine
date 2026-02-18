#include "EnemyBullet.h"
#include "Collision/CollisionTypeIDDef.h"
#include "MathVector.h"

#include <imgui.h>

using namespace Engine;
using namespace MathVector;

void EnemyBullet::Initialize() {

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// モデルの生成・初期化
	model = std::make_unique<Model>();
	model->Initialize("EnemyBullet", "EnemyBullet.obj");

	// 3Dオブジェクトの生成・初期化
	object = std::make_unique<Object3d>();
	object->Initialize();
	object->SetModel(model.get());
	object->SetScale({ 0.5f, 0.5f, 0.5f });

	isDead = false;

	// コライダーの形状を球に設定
	colliderShape_ = CreateSphere(worldTransform_.GetWorldPosition(), 0.5f);

	// コライダーの初期化
	Collider::Initialize();
	// コライダーにIDを設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIDDef::kEnemyBullet));
	// コライダーに形状を設定
	Collider::SetSphere(colliderShape_);

	// エミッターの生成
	particleEmitter = std::make_unique<ParticleEmitter>("BulletRed", EmitterType::OneShot, 20);
	particleEmitter->Initialize();
};

void EnemyBullet::Update() {

	// 速度は向きだけもらってきたから正規化して速さをかける
	velocity_ = Normalize(velocity_) * moveSpeed;

	// ワールド変換の平行移動に速度を加算
	worldTransform_.AddTranslate(velocity_);

	object->SetTranslate(worldTransform_.GetTranslate());

	if (deathTimer_ <= 0) {

		isDead = true;
	}
	else {

		deathTimer_--;
	}

	worldTransform_.UpdateMatrix();

	// コライダーの更新
	colliderShape_ = CreateSphere(worldTransform_.GetWorldPosition(), 0.5f);
	Collider::SetSphere(colliderShape_);

	object->Update();
};

void EnemyBullet::Draw() {

	object->Draw();
};

void EnemyBullet::Finalize() {
}

void EnemyBullet::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("Bullet");

	worldTransform_.ShowImGui();

	ImGui::Text("Velocity: (%.2f, %.2f, %.2f)", velocity_.x, velocity_.y, velocity_.z);

	ImGui::End();

#endif // USE_IMGUI
}

void EnemyBullet::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が自機の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kPlayer)) {

		// エミッターの位置を設定
		particleEmitter->SetTranslate(worldTransform_.GetWorldPosition());

		// パーティクル発生
		particleEmitter->Emit();

		isDead = true;
	}
	// 衝突相手が自機の弾の場合
	else if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kPlayerBullet)) {

		// エミッターの位置を設定
		particleEmitter->SetTranslate(worldTransform_.GetWorldPosition());

		// パーティクル発生
		particleEmitter->Emit();

		isDead = true;
	}
	// その他と衝突した場合
	else {

		// 何もしない
		return;
	}
}
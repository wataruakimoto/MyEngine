#include "Bullet.h"
#include "Collision/CollisionTypeIDDef.h"
#include "MathVector.h"

#include <imgui.h>

using namespace MathVector;

void Bullet::Initialize() {

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// モデルの生成・初期化
	model = std::make_unique<Model>();
	model->Initialize("PlayerBullet", "PlayerBullet.obj");

	// 3Dオブジェクトの生成・初期化
	object = std::make_unique<Object3d>();
	object->Initialize();
	object->SetModel(model.get());
	object->SetScale({ 0.2f, 0.2f, 0.8f });

	isDead = false;

	// コライダーの初期化
	Collider::Initialize();
	// コライダーにIDを設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIDDef::kPlayerBullet));

	// エミッターの生成・初期化
	particleEmitter = std::make_unique<ParticleEmitter>("BulletBlue", 0.0f, 20);
	particleEmitter->Initialize();
};

void Bullet::Update() {

	// 速度は向きだけもらってきたから速さをかける
	velocity_ = velocity_ * moveSpeed;

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

	object->Update();

	// エミッターの更新
	particleEmitter->Update();
};

void Bullet::Draw() {

	object->Draw();
};

void Bullet::Finalize() {
}

void Bullet::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("Bullet");

	worldTransform_.ShowImGui();

	ImGui::End();

#endif // USE_IMGUI
}

void Bullet::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が敵の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemy)) {

		// エミッターの位置を設定
		particleEmitter->SetTranslate(worldTransform_.GetTranslate());

		// パーティクル発生
		particleEmitter->Emit();

		isDead = true;
	}
	// 衝突相手が敵の弾の場合
	else if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemyBullet)) {

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

void Bullet::Move() {

	Vector3 direction = Normalize(velocity_);

}

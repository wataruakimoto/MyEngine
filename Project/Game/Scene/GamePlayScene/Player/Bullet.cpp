#include "Bullet.h"
#include "Scene/GamePlayScene/Collision/CollisionTypeIDDef.h"
#include "Math/MathVector.h"
#include <imgui.h>

void Bullet::Initialize() {

	// 基底クラスの初期化
	Basecharacter::Initialize();

	// モデルの生成・初期化
	model = new Model();
	model->Initialize("resources/bullet", "bullet.obj");

	// 3Dオブジェクトの生成・初期化
	object = new Object3d();
	object->Initialize();
	object->SetModel(model);
	object->SetScale({ 0.5f, 0.5f, 0.5f });

	isDead = false;

	// コライダーの初期化
	Collider::Initialize();
	// コライダーにIDを設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIDDef::kBullet));
};

void Bullet::Update() {

	// 速度は向きだけもらってきたから正規化して速さをかける
	velocity_ = Normalize(velocity_) * moveSpeed;

	transform_.translate += velocity_;

	object->SetTranslate(transform_.translate);

	if (deathTimer_ <= 0) {

		isDead = true;
	}
	else {

		deathTimer_--;
	}

	object->Update();
};

void Bullet::Draw() {

	object->Draw();
};

void Bullet::Finalize() {

	// 3Dオブジェクトの解放
	delete object;

	// モデルの解放
	delete model;
}

void Bullet::ShowImGui() {

#ifdef _DEBUG

	ImGui::Begin("Bullet");

	ImGui::DragFloat3("Scale", &transform_.scale.x, 0.1f);
	ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &transform_.translate.x, 0.1f);
	
	ImGui::End();

#endif // _DEBUG
}

void Bullet::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が敵の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemy)) {

		isDead = true;
	}
	// その他と衝突した場合
	else {

		// 何もしない
		return;
	}
}
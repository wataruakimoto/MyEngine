#include "Enemy.h"
#include "math/MathVector.h"
#include "Scene/GamePlayScene/Collision/CollisionTypeIDDef.h"
#include "Scene/GamePlayScene/Player/Player.h"
#include <numbers>
#include <imgui.h>

void Enemy::Initialize() {

	// 基底クラスの初期化
	Basecharacter::Initialize();

	// モデルの生成・初期化
	model = new Model();
	model->Initialize("resources/enemy", "enemy.obj");
	// 3Dオブジェクトの生成・初期化
	object = new Object3d();
	object->Initialize();
	object->SetModel(model);
	object->SetScale({ 1.0f, 1.0f, 1.0f });
	object->SetTranslate({ 10.0f, 10.0f, 50.0f });

	isDead = false;

	// コライダーの初期化
	Collider::Initialize();
	// コライダーにIDを設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIDDef::kEnemy));

	// Transformの設定
	EmitterTransform1.translate = { 0.0f,2.0f,0.0f };
	// パーティクルの設定
	particleSetting1.randomizeScale = true;
	particleSetting1.randomScaleMin = { 0.2f,1.0f,1.0f };
	particleSetting1.randomScaleMax = { 0.2f,3.0f,1.0f };
	particleSetting1.randomizeRotate = true;
	particleSetting1.randomRotateMin = { 0.0f,0.0f,-std::numbers::pi_v<float> };
	particleSetting1.randomRotateMax = { 0.0f,0.0f,std::numbers::pi_v<float> };
	// エミッタ生成
	particleEmitter1 = std::make_unique <ParticleEmitter>("circle2", EmitterTransform1, 3, 0.0f, particleSetting1);
}

void Enemy::Update() {

	worldTransform_.rotate = object->GetRotate();
	worldTransform_.translate = object->GetTranslate();

#ifdef _DEBUG

	ImGui::Begin("Enemy");

	ImGui::DragFloat3("Scale", &worldTransform_.scale.x, 0.1f);
	ImGui::DragFloat3("Rotate", &worldTransform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_.translate.x, 0.1f);

	ImGui::End();

#endif // DEBUG_

	// プレイヤーの座標を取得
	Vector3 playerPos = player->GetTranslate();

	// プレイヤーとの方向を計算
	Vector3 direction = playerPos - worldTransform_.translate;
	float length = Length(direction);
	direction= { direction.x / length, direction.y / length, direction.z / length };

	// 向きを変える
	worldTransform_.rotate = { 0.0f,std::atan2(direction.x,direction.z),0.0f };

	// プレイヤーに向かって進む
	worldTransform_.translate += {direction.x* moveSpeed, direction.y* moveSpeed, direction.z* moveSpeed};

	object->SetTranslate(worldTransform_.translate);
	object->SetRotate(worldTransform_.rotate);

	// 3Dオブジェクトの更新
	object->Update();
}

void Enemy::Draw() {

	// 3Dオブジェクトの描画
	object->Draw();
}

void Enemy::Finalize() {

	// 3Dオブジェクトの解放
	delete object;

	// モデルの解放
	delete model;
}

void Enemy::ShowImGui() {

#ifdef _DEBUG

	ImGui::Begin("Enemy");

	ImGui::DragFloat3("Scale", &worldTransform_.scale.x, 0.1f);
	ImGui::DragFloat3("Rotate", &worldTransform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_.translate.x, 0.1f);

	ImGui::End();

#endif // _DEBUG
}

void Enemy::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	
	// 衝突相手が弾の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kBullet)) {

		// パーティクル発生
		EmitterTransform1.translate = worldTransform_.translate;
		particleEmitter1->SetTransform(EmitterTransform1);
		particleEmitter1->Emit();

		// 死亡
		isDead = true;
	}
	// 衝突相手がプレイヤーの場合
	else if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kPlayer)) {
		// 死亡
		isDead = true;
	}
	// その他と衝突した場合
	else {

		// 何もしない
		return;
	}
}
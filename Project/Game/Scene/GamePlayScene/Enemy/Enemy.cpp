#include "Enemy.h"
#include "math/MathVector.h"
#include "Scene/GamePlayScene/Collision/CollisionTypeIDDef.h"
#include "Scene/GamePlayScene/Player/Player.h"
#include "Scene/GamePlayScene/GamePlayScene.h"
#include "EnemyBullet.h"
#include <numbers>
#include <imgui.h>

using namespace MathVector;

void Enemy::Initialize() {

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// モデルの生成・初期化
	model = std::make_unique<Model>();
	model->Initialize("resources/enemy", "enemy.obj");
	// 3Dオブジェクトの生成・初期化
	object = std::make_unique<Object3d>();
	object->Initialize();
	object->SetModel(model.get());
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
	particleEmitter1 = std::make_unique<ParticleEmitter>("circle2", EmitterTransform1, 3, 0.0f, particleSetting1);
}

void Enemy::Update() {

	// 自機の方に向ける
	AimToPlayer();

	// 射撃
	if (fireTimer <= 0) {

		// プレイヤーより手前にいたら
		if (worldTransform_.GetTranslate().z > player->GetWorldTransform().GetTranslate().z) {

			Fire();
			fireTimer = 60.0f;
		}
	}
	else {

		fireTimer--;
	}

	object->SetTranslate(worldTransform_.GetTranslate());
	object->SetRotate(worldTransform_.GetRotate());

	worldTransform_.UpdateMatrix();

	// 3Dオブジェクトの更新
	object->Update();
}

void Enemy::Draw() {

	// 3Dオブジェクトの描画
	object->Draw();
}

void Enemy::Finalize() {
}

void Enemy::ShowImGui() {

#ifdef _DEBUG

	ImGui::Begin("Enemy");

	worldTransform_.ShowImGui();

	ImGui::End();

#endif // _DEBUG
}

void Enemy::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が弾の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kPlayerBullet)) {

		// パーティクル発生
		EmitterTransform1.translate = worldTransform_.GetTranslate();
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

void Enemy::AimToPlayer() {

	// プレイヤーの座標を取得
	Vector3 playerPos = player->GetWorldTransform().GetWorldPosition();

	// プレイヤーとの方向を計算
	Vector3 direction = playerPos - worldTransform_.GetTranslate();
	direction = Normalize(direction);

	// X軸（ピッチ）：上下方向
	float pitch = std::atan2(-direction.y, std::sqrt(direction.x * direction.x + direction.z * direction.z));
	// Y軸（ヨー）：左右方向
	float yaw = std::atan2(direction.x, direction.z);
	// Z軸（ロール）：通常は0でOK
	float roll = 0.0f;

	// 向きを変える
	worldTransform_.SetRotate({ pitch, yaw, roll });
}

void Enemy::Fire() {

	// 弾の生成&初期化
	std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize();

	// 弾の初期位置をプレイヤーの位置に設定
	bullet->GetWorldTransform().SetTranslate(worldTransform_.GetWorldPosition());

	// プレイヤーの座標を取得
	Vector3 playerPos = player->GetWorldTransform().GetWorldPosition();

	// 弾の初期速度を設定
	// プレイヤーとの方向を計算
	Vector3 direction = playerPos - worldTransform_.GetTranslate();
	direction = Normalize(direction);
	bullet->SetVelocity(direction);

	// ゲームプレイシーンの弾をリストに登録
	gamePlayScene_->AddEnemyBullet(std::move(bullet));
}

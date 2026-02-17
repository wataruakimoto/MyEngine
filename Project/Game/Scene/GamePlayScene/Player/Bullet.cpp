#include "Bullet.h"
#include "Collision/CollisionTypeIDDef.h"
#include "MathVector.h"

#include <imgui.h>

using namespace Engine;
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
	object->SetScale({ 0.5f, 0.5f, 5.0f });

	// デスタイマーの初期化
	deathTimer_ = 0.0f;

	// 死んだかどうかのフラグの初期化
	isDead = false;

	// コライダーの初期化
	Collider::Initialize();
	// コライダーにIDを設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIDDef::kPlayerBullet));

	// エミッターの生成・初期化
	particleEmitter = std::make_unique<ParticleEmitter>("BulletBlue", EmitterType::OneShot, 20);
	particleEmitter->Initialize();
};

void Bullet::Update() {

	// デスタイマーの更新
	deathTimer_ += 1.0f / 60.0f;

	// デスタイマーが寿命を超えたら
	if (deathTimer_ >= kLifeTime) {

		isDead = true;
	}

	// 移動処理
	Move();

	// ワールド変換の更新
	worldTransform_.UpdateMatrix();

	// 3Dオブジェクトの更新
	object->SetRotate(worldTransform_.GetRotate());
	object->SetTranslate(worldTransform_.GetTranslate());
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

		// ロックオンでの射撃の場合
		if (isLockedOn_) {

			return;
		}

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

	/// ===== 向きの処理 ===== ///

	// ヨー(Y軸回りの回転)の計算
	float yaw = atan2f(direction_.x, direction_.z);

	// 水壁距離の計算
	float horizontalLength = Length(direction_.x, direction_.z);

	// ピッチ(X軸回りの回転)の計算
	float pitch = atan2f(-direction_.y, horizontalLength);

	// 回転の設定
	worldTransform_.SetRotate({ pitch, yaw, 0.0f });

	/// ===== 速度の処理 ===== ///

	// 速度に向きをかける
	velocity_ = direction_ * moveSpeed;

	// ワールド変換の平行移動に速度を加算
	worldTransform_.AddTranslate(velocity_);
}

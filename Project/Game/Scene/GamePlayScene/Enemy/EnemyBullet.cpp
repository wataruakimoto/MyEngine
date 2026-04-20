#include "EnemyBullet.h"
#include "Collision/CollisionTypeIDDef.h"
#include "MathVector.h"

#include <imgui.h>

using namespace Engine;
using namespace MathVector;

void EnemyBullet::Initialize() {

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.SetScale({ 1.0f, 1.0f, 5.0f });

	// モデルの生成・初期化
	model = std::make_unique<Model>();
	model->Initialize("EnemyBullet", "EnemyBullet.obj");

	// 3Dオブジェクトの生成・初期化
	object = std::make_unique<Object3d>();
	object->Initialize();
	object->SetModel(model.get());
	object->GetWorldTransform().SetParent(&worldTransform_);

	isDead = false;

	// コライダーの生成
	collider_ = std::make_unique<Collider>(
		OBB{},
		static_cast<uint32_t>(CollisionTypeIDDef::kEnemyBullet)
	);
	// コライダーの初期化
	collider_->Initialize();
	// コライダーに衝突時のコールバック関数を設定
	collider_->SetOnCollision([this](Collider* other) { OnCollision(other); });
	// コライダーにワールド変換を設定
	collider_->GetWorldTransform().SetParent(&worldTransform_);

	// エミッターの生成
	particleEmitter = std::make_unique<ParticleEmitter>("BulletRed", EmitterType::OneShot, 20);
	particleEmitter->Initialize();
};

void EnemyBullet::Update() {

	// デスタイマーの更新
	deathTimer_ += 1.0f / 60.0f;

	// デスタイマーが寿命を超えたら
	if (deathTimer_ >= kLifeTime) {

		isDead = true;
	}

	// 移動処理
	Move();

	worldTransform_.Update();

	// コライダーの更新
	collider_->Update();

	object->Update();

	// エミッターの更新
	particleEmitter->Update();
};

void EnemyBullet::Draw() {

	// コライダーの描画
	collider_->Draw();

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
	// 衝突相手が障害物の場合
	else if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kObstacle)) {

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

void EnemyBullet::Move() {

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

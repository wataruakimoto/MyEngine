#include "Enemy.h"
#include "Collision/CollisionTypeIDDef.h"
#include "Player/Player.h"
#include "GamePlayScene.h"
#include "EnemyBullet.h"
#include "MathVector.h"
#include "Easing.h"

#include <numbers>
#include <imgui.h>

using namespace MathVector;
using namespace Easing;

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

	// エミッタ生成
	particleEmitterBlack = std::make_unique<ParticleEmitter>("EnemyDeathBlack", 0.0f, 40);
	particleEmitterWhite = std::make_unique<ParticleEmitter>("EnemyDeathWhite", 0.0f, 10);
}

void Enemy::Update() {

	// 自機の方に向ける
	AimToPlayer();

	// 射撃
	if (fireTimer_ <= 0) {

		// プレイヤーより手前にいたら
		if (worldTransform_.GetTranslate().z > player->GetWorldTransform().GetTranslate().z) {

			// プレイヤーがマニュアル操作中なら
			if (player->GetState() == PlayerState::Manual) {

				// 射撃
				Fire();
			}
		}
	}
	else {

		// タイマーをデクリメント
		fireTimer_ -= 1.0f / 60.0f;
	}

	if (isFiring_) {

		// 射撃アニメーション更新
		FireAnimationUpdate();
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

#ifdef USE_IMGUI

	ImGui::Begin("Enemy");

	worldTransform_.ShowImGui();

	ImGui::End();

#endif // USE_IMGUI
}

void Enemy::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が弾の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kPlayerBullet)) {

		// エミッターの位置を設定
		particleEmitterBlack->SetTranslate(worldTransform_.GetWorldPosition());
		particleEmitterWhite->SetTranslate(worldTransform_.GetWorldPosition());

		// パーティクル発生
		particleEmitterWhite->Emit();
		particleEmitterBlack->Emit();

		gamePlayScene_->OnEnemyDefeated();

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

	// 射撃間隔タイマーをリセット
	fireTimer_ = kFireDuration_;

	// 射撃アニメーション開始
	isFiring_ = true;
	fireAnimationTimer_ = kFireAnimationDuration_; // アニメーションタイマーをリセット
	object->SetScale(fireScale_);
	object->SetScale(fireScale_);
}

void Enemy::FireAnimationUpdate() {

	// デルタタイム分デクリメント
	fireAnimationTimer_ -= 1.0f / 60.0f;

	float t = 1.0f - (fireAnimationTimer_ / kFireAnimationDuration_); // 経過割合を計算
	float easedT = EaseOutCubic(t); // イージング適用
	Vector3 newScale = Lerp(fireScale_, defaultScale_, easedT); // スケールを補間

	// スケールを設定
	object->SetScale(newScale);

	// タイマーが0以下になったら
	if (fireAnimationTimer_ <= 0.0f) {

		isFiring_ = false; // 射撃アニメーション終了
		newScale = defaultScale_; // スケールをデフォルトに戻す
	}
}

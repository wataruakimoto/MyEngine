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

	switch (enemyType_) {
	case EnemyType::Normal:
	default:

		model->Initialize("resources/enemy", "enemy.obj");

		break;

	case EnemyType::Kamikaze:

		model->Initialize("resources/enemy", "Kamikaze.obj");

		break;
	}

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

	Vector3 playerPos = player->GetWorldTransform().GetWorldPosition();
	Vector3 myPos = worldTransform_.GetWorldPosition();

	// 距離を計算
	float distanceToPlayer = Distance(playerPos, myPos);

	// 行動開始距離
	const float kActiveDistance = 150.0f;

	switch (enemyType_) {

	case EnemyType::Normal:
	default:

		// 自機の方に向ける
		AimToPlayer();

		// プレイヤーからの距離が一定以内の場合
		if (distanceToPlayer <= kActiveDistance) {

			// 射撃
			if (fireTimer_ <= 0) {

				// プレイヤーより奥にいたら
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
		}

		break;

	case EnemyType::Kamikaze:

		// プレイヤーより奥にいたら
		if (worldTransform_.GetTranslate().z > player->GetWorldTransform().GetTranslate().z) {

			// 前進
			MoveForward();
		}
		else {

			worldTransform_.AddTranslate({ 0.0f,0.0f,-0.5f });
		}

		break;
	}

	// ワールド変換の更新
	worldTransform_.UpdateMatrix();

	object->SetTranslate(worldTransform_.GetTranslate());
	object->SetRotate(worldTransform_.GetRotate());

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

void Enemy::MoveForward() {

	// 1. プレイヤーへの「理想の方向」を計算
	Vector3 playerPos = player->GetWorldTransform().GetWorldPosition();
	Vector3 myPos = worldTransform_.GetTranslate();
	Vector3 targetDir = playerPos - myPos;
	targetDir = Normalize(targetDir); // 正規化

	// 2. 現在の速度ベクトルを、理想の方向へ少しずつ近づける（ここが重要！）

	// 追従性能（0.01f:曲がれない ～ 1.0f:即座に向く）
	// 0.05f くらいだと「避けられるけど追いかけてくる」いい感じになります
	float turnSpeed = 0.8f;

	// 距離が近すぎるときは追尾をやめる（回避しやすくする隠し味）
	// これを入れると、直前で「あきらめて直進」するので避けやすくなります
	float distance = Length(playerPos - myPos);
	if (distance < 30.0f) {
		turnSpeed = 0.0f; // 追尾停止
	}

	// ベクトルを補間する
	velocity_ = Lerp(velocity_, targetDir, turnSpeed);
	velocity_ = Normalize(velocity_); // 長さが変わってしまうので再度正規化


	// 3. 移動（保存しておいた velocity_ を使う）
	float speed = 0.8f;
	worldTransform_.AddTranslate(velocity_ * speed);


	// 4. 回転の計算（velocity_ を元に計算）
	float yaw = std::atan2(velocity_.x, velocity_.z);
	float pitch = -std::atan2(velocity_.y, std::sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z));

	// ドリル回転
	float rotationSpeed = 0.2f;
	drillRotation_ += rotationSpeed;
	if (drillRotation_ > std::numbers::pi_v<float> *2.0f) {
		drillRotation_ -= std::numbers::pi_v<float> *2.0f;
	}

	// 向きをセット
	worldTransform_.SetRotate({ pitch, yaw, drillRotation_ });
}

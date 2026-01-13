#include "Player.h"
#include "Input.h"
#include "GamePlayScene.h"
#include "Bullet.h"
#include "Collision/CollisionTypeIDDef.h"
#include "Reticle/Reticle3D.h"
#include "MathVector.h"
#include "Easing.h"

#include <list>
#include <algorithm>
#include <imgui.h>

using namespace MathVector;
using namespace Easing;

void Player::Initialize() {

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.SetTranslate({ 0.0f, 4.0f, 0.0f });

	// モデルの生成・初期化
	model = std::make_unique<Model>();
	model->Initialize("Player", "player.obj");

	// 3Dオブジェクトの生成・初期化
	object = std::make_unique<Object3d>();
	object->Initialize();
	object->SetModel(model.get());
	object->SetScale({ 1.0f, 1.0f, 1.0f });

	// コライダーの初期化
	Collider::Initialize();
	// コライダーにIDを設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIDDef::kPlayer));

	isDead_ = false;
}

void Player::Update() {

	// 状態の変更がリクエストされていたら
	if (stateRequest_) {

		// 状態を変更
		state_ = stateRequest_.value();

		// 各状態の初期化を行う
		switch (state_) {

		case PlayerState::AutoPilot:

			// オートパイロットモードの初期化処理
			AutoPilotInitialize();

			break;

		case PlayerState::Manual:

			// マニュアルモードの初期化処理
			ManualInitialize();

			break;

		case PlayerState::Rolling:

			// バレルロールモードの初期化処理
			RollingInitialize();

			break;

		case PlayerState::Dead:

			// 死亡モードの初期化処理
			DeadInitialize();

			break;

		default:

			break;
		}

		// リクエストをクリア
		stateRequest_ = std::nullopt;
	}

	// 将来的にはWSwitchから基底と継承先を分ける
	switch (state_) {

	case PlayerState::AutoPilot:

		// オートパイロットモードの更新
		AutoPilotUpdate();

		break;

	case PlayerState::Manual:

		// マニュアルモードの更新
		ManualUpdate();

		break;

	case PlayerState::Rolling:

		// バレルロールモードの更新
		RollingUpdate();

		break;

	case PlayerState::Dead:

		// 死亡モードの更新
		DeadUpdate();

		break;
	}

	// ワールド変換の更新
	worldTransform_.UpdateMatrix();

	screenPos_ = ConvertWorldToScreen(worldTransform_.GetWorldPosition(), camera_->GetViewProjectionMatrix());

	object->SetTranslate(worldTransform_.GetWorldPosition());
	object->SetRotate(worldTransform_.GetRotate());

	// 3Dオブジェクトの更新
	object->Update();
}

void Player::Draw() {

	if (!isGroundHit_) {

		// 3Dオブジェクトの描画
		object->Draw();
	}
}

void Player::Finalize() {
}

void Player::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("Player");

	worldTransform_.ShowImGui();

	ImGui::SliderFloat3("Velocity", &velocity_.x, -0.2f, 0.2f);

	ImGui::Text("ScreenPos: (%.2f, %.2f)", screenPos_.x, screenPos_.y);

	// 状態の表示
	ImGui::Text("State: %s", (state_ == PlayerState::AutoPilot) ? "AutoPilot" :
		(state_ == PlayerState::Manual) ? "Manual" :
		(state_ == PlayerState::Dead) ? "Dead" : "Unknown");

	ImGui::Text("speedTitle: %.2f", moveSpeedAuto);

	ImGui::Text("speedPlay: %.2f", moveSpeedManual);

	object->ShowImGui();

	model->ShowImGui();

	ImGui::End();

#endif // USE_IMGUI
}

void Player::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 状態がバレルロール中の場合
	if (state_ == PlayerState::Rolling) {

		// 衝突相手が敵の場合
		if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemy)) {

			// 1ダメージを受ける
			DamageProcess(1);
		}
		// 衝突相手が敵の弾の場合
		else if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemyBullet)) {

			// 何もしない
			return;
		}
		// その他と衝突した場合
		else {

			// 何もしない
			return;
		}
	}

	// 衝突相手が敵の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemy)) {

		// 1ダメージを受ける
		DamageProcess(1);
	}
	// 衝突相手が敵の弾の場合
	else if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemyBullet)) {

		// 1ダメージを受ける
		DamageProcess(1);
	}
	// その他と衝突した場合
	else {

		// 何もしない
		return;
	}
}

void Player::OnGateCollision() {

	// すでにやられていたら何もしない
	if (state_ == PlayerState::Dead) return;

	// 死亡状態に変更をリクエスト
	stateRequest_ = PlayerState::Dead;
}

void Player::Fire() {

	// 弾の生成&初期化
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
	bullet->Initialize();

	// 弾の初期位置をプレイヤーの位置に設定
	bullet->GetWorldTransform().SetTranslate(worldTransform_.GetWorldPosition());

	// 弾の初期速度を設定
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	// ロックオン中なら
	if (lockOn_->IsLockOn()) {

		// ターゲットの位置
		Vector3 targetPos = lockOn_->GetTarget()->GetWorldTransform().GetWorldPosition();

		// ターゲットの位置までの方向ベクトルを求める
		velocity = targetPos - worldTransform_.GetWorldPosition();

		// 正規化
		velocity = Normalize(velocity);

		// 速度を設定
		bullet->SetVelocity(velocity);
	}
	// ロックオンしていないなら
	else {

		// レティクルの位置
		Vector3 reticlePos = reticle3D_->GetWorldTransform().GetWorldPosition();

		// レティクルの位置までの方向ベクトルを求める
		velocity = reticlePos - worldTransform_.GetWorldPosition();

		// 正規化
		velocity = Normalize(velocity);

		// 速度を設定
		bullet->SetVelocity(velocity);
	}

	// ゲームプレイシーンの弾をリストに登録
	gamePlayScene_->AddPlayerBullet(std::move(bullet));

	// 射撃間隔タイマーをリセット
	fireTimer_ = kFireDuration_;

	// 射撃アニメーション開始
	isFiring_ = true;
	fireAnimationTimer_ = kFireAnimationDuration_; // アニメーションタイマーをリセット
	object->SetScale(fireScale_);
	object->SetScale(fireScale_);
}

void Player::FireAnimationUpdate() {

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

void Player::MoveToReticle() {

	// レティクルの位置を取得
	Vector3 reticlePos = reticle3D_->GetWorldTransform().GetWorldPosition();

	// レティクルの方向ベクトルを求める
	Vector3 toReticle = reticlePos - worldTransform_.GetWorldPosition();

	// 正規化
	toReticle = Normalize(toReticle);

	// 横軸の長さを求める
	float xzLength = Length(toReticle.x, toReticle.z);

	// ヨー(Y軸回りの回転)を求める
	float yaw = atan2f(toReticle.x, toReticle.z);

	// ピッチ(X軸回りの回転)を求める
	float pitch = atan2f(-toReticle.y, xzLength);

	// 回転を設定
	worldTransform_.SetRotate({ pitch, yaw, 0.0f });

	// 座標に速度を加算
	worldTransform_.AddTranslate(toReticle * moveSpeedManual);

	// 大きさを取得
	Vector3 scale = worldTransform_.GetScale();

	// 加算後の座標を取得
	Vector3 currentPos = worldTransform_.GetTranslate();

	// X軸のクランプ
	currentPos.x = std::clamp(currentPos.x, kMoveMin.x + scale.x, kMoveMax.x - scale.y);
	// Y軸のクランプ
	currentPos.y = std::clamp(currentPos.y, kMoveMin.y + scale.y, kMoveMax.y - scale.y);

	// クランプ後の座標を設定
	worldTransform_.SetTranslate(currentPos);
}

void Player::DamageProcess(uint16_t damage) {

	// 状態が死亡状態でなければ
	if (state_ != PlayerState::Dead) {

		// HPが0より大きいなら
		if (hp_ > 0) {
			
			// ダメージ分HPを減らす
			hp_ -= damage;

			// シーンにダメージを通知
			if (gamePlayScene_) {
				gamePlayScene_->OnPlayerDamaged(hp_);
			}
		}

		// HPが0なら死亡状態に変更をリクエスト
		if (hp_ == 0) {
			stateRequest_ = PlayerState::Dead;
		}
	}
}

void Player::AutoPilotInitialize() {
}

void Player::AutoPilotUpdate() {

	// 現在の回転を取得
	Vector3 currentRotate = worldTransform_.GetRotate();
	Vector3 targetRotate = { 0.0f, 0.0f, 0.0f }; // 正面を向く

	// 回転を補間
	currentRotate = Lerp(currentRotate, targetRotate, 0.1f);

	// ほとんど目標回転に近づいたら
	if (Length(currentRotate - targetRotate) < 0.01f) {
		currentRotate = targetRotate; // 目標回転に設定
	}

	// 回転を設定
	worldTransform_.SetRotate(currentRotate);

	// Z方向のみの移動
	worldTransform_.AddTranslate({ 0.0f, 0.0f, moveSpeedAuto });
}

void Player::ManualInitialize() {

	// 射撃のクールタイマーをリセット
	fireTimer_ = kFireDuration_;

	// バレルロールのクールタイマーをリセット
	rollCooldownTimer_ = kRollCooldownDuration_;
}

void Player::ManualUpdate() {

	// 各キーの入力の状態
	bool isSpacePush = Input::GetInstance()->PushKey(DIK_SPACE); // スペースキー
	bool isAPush = Input::GetInstance()->PushKey(DIK_A); // Aキー
	bool isDPush = Input::GetInstance()->PushKey(DIK_D); // Dキー

	/// ===== 射撃処理 ===== ///

	// タイマーが0以下なら
	if (fireTimer_ <= 0) {

		// スペースキーが押されたら
		if (isSpacePush) {

			// 射撃
			Fire();
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

	/// ===== バレルロール処理 ===== ///

	// タイマーが0以下なら
	if (rollCooldownTimer_ <= 0.0f) {

		// Aキーだけ押された場合
		if (isAPush && !isDPush) {

			rollDirection_ = -1; // 左回り

			// 状態変更をリクエスト
			stateRequest_ = PlayerState::Rolling;
		}
		// Dキーだけ押された場合
		else if (isDPush && !isAPush) {

			rollDirection_ = 1; // 右回り

			// 状態変更をリクエスト
			stateRequest_ = PlayerState::Rolling;
		}
	}
	else {

		// タイマーをデクリメント
		rollCooldownTimer_ -= 1.0f / 60.0f;
	}

	/// ===== 移動処理 ===== ///

	// レティクルに向かって移動
	MoveToReticle();
}

void Player::RollingInitialize() {

	rollTimer_ = 0.0f;

	velocity_ = { 0.0f, 0.0f, 0.0f };
}

void Player::RollingUpdate() {

	// タイマーを進める
	rollTimer_ += 1.0f / 60.0f; // デルタタイム加算

	// 進行度を計算
	float t = rollTimer_ / rollDuration_;

	// タイマーが最大値に達したら
	if (t >= 1.0f) {

		// ロール完了
		t = 1.0f;

		// 状態をマニュアルに戻す
		stateRequest_ = PlayerState::Manual;

		// 回転をリセット
		Vector3 currentRotate = worldTransform_.GetRotate();
		currentRotate.z = 0.0f;
		worldTransform_.SetRotate(currentRotate);

		return; // 以降の処理をスキップ
	}
	
	/// ===== 回転の計算 ===== ///

	// イージング適用
	float easeT = EaseOutCubic(t);

	// 現在の回転角度を計算
	float currentAngle = (float)rollDirection_ * -1.0f * kMaxRollAngle_ * easeT; // -1.0fをかけて回転方向を反転

	// 回転を設定
	Vector3 currentRotate = worldTransform_.GetRotate();
	currentRotate.z = currentAngle;
	worldTransform_.SetRotate(currentRotate);

	/// ===== 移動の計算 ===== ///

	// 強制的に横移動する
	velocity_.x = (float)rollDirection_ * kRollMoveSpeed_; // 横移動速度を設定
	velocity_.y = 0.0f; // 縦移動速度は0
	velocity_.z = kRollMoveSpeed_; // 前進速度を設定

	// 座標の更新
	worldTransform_.AddTranslate(velocity_);

	/// ===== 画面外に出ないようにする処理 ===== ///

	// 大きさを取得
	Vector3 scale = worldTransform_.GetScale();

	// 加算後の座標を取得
	Vector3 currentPos = worldTransform_.GetTranslate();

	// X軸のクランプ
	currentPos.x = std::clamp(currentPos.x, kMoveMin.x + scale.x, kMoveMax.x - scale.y);

	// クランプ後の座標を設定
	worldTransform_.SetTranslate(currentPos);
}

void Player::DeadInitialize() {

	// タイマーをリセット
	deathTimer_ = 0.0f;

	// 落下速度・回転速度を設定
	deathVelocity_ = { 0.0f, kFallStartSpeed, 0.0f };
	deathRotateVelocity_ = { kRollSpeed, 0.0f, kRollSpeed * 0.5f };

	// 操作・弾発射を無効化
	velocity_ = { 0.0f, 0.0f, 0.0f };

	isGroundHit_ = false;

	isParticleEmitted_ = false;

	// エミッターの生成
	particleEmitterRed = std::make_unique<ParticleEmitter>("PlayerDeathRed", 0.0f, 10);
	particleEmitterBlue = std::make_unique<ParticleEmitter>("PlayerDeathBlue", 0.0f, 40);
}

void Player::DeadUpdate() {

	// タイマーを進める
	deathTimer_ += 1.0f / 60.0f; // デルタタイム加算

	// 5秒経過したらタイトルシーンに戻る
	if (deathTimer_ >= 2.0f) {
		isDead_ = true;
		return; // 以降の処理をスキップ
	}

	// 回転速度の加算
	deathRotateVelocity_.x += kRollAcceleration;
	deathRotateVelocity_.z += kRollAcceleration * 0.5f;

	// 回転の更新
	worldTransform_.AddRotate(deathRotateVelocity_);

	// 落下処理
	deathVelocity_.y += kFallAcceleration;

	// 落下が既定値より速くなったら
	if (deathVelocity_.y < kMaxFallSpeed) {

		// 最大値に揃える
		deathVelocity_.y = kMaxFallSpeed;
	}

	// 横揺れの計算
	float swayX = sinf(deathTimer_ * kSwayFrequency) * kSwayAmplitude;
	float swayZ = cosf(deathTimer_ * kSwayFrequency * 0.5f) * kSwayAmplitude;

	// 座標の計算
	Vector3 position = { swayX * 0.05f, deathVelocity_.y, swayZ * 0.05f };

	// 座標の更新
	worldTransform_.AddTranslate(position);

	// 地面に到達したら
	if (worldTransform_.GetWorldPosition().y <= kGroundHeight + worldTransform_.GetScale().y) {

		Vector3 Translate = { worldTransform_.GetWorldPosition().x, kGroundHeight + worldTransform_.GetScale().y, worldTransform_.GetWorldPosition().z };

		// Y座標を地面の高さに揃える
		worldTransform_.SetTranslate(Translate);

		// フラグを立てる
		isGroundHit_ = true;

		if (!isParticleEmitted_) {

			// エミッターの位置を設定
			particleEmitterRed->SetTranslate(worldTransform_.GetWorldPosition());
			particleEmitterBlue->SetTranslate(worldTransform_.GetWorldPosition());

			// パーティクルを発生させる
			particleEmitterRed->Emit();
			particleEmitterBlue->Emit();

			isParticleEmitted_ = true;
		}
	}
}

#include "Player.h"
#include "Input.h"
#include "GamePlayScene.h"
#include "Bullet.h"
#include "Collision/CollisionTypeIDDef.h"
#include "Reticle/Reticle3D.h"
#include "MathVector.h"

#include <list>
#include <imgui.h>

using namespace MathVector;

void Player::Initialize() {

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.SetTranslate({ 0.0f, 4.0f, 0.0f });

	// モデルの生成・初期化
	model = std::make_unique<Model>();
	model->Initialize("resources/player", "player.obj");

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

#ifdef _DEBUG

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

	particleEmitterRed->ShowImGui("ParticleEmitterRed");

	particleEmitterBlue->ShowImGui("ParticleEmitterBlue");

#endif // _DEBUG
}

void Player::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が敵の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemy)) {

		// 死亡状態に変更をリクエスト
		stateRequest_ = PlayerState::Dead;
	}
	// 衝突相手が敵の弾の場合
	else if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemyBullet)) {

		// 死亡状態に変更をリクエスト
		stateRequest_ = PlayerState::Dead;
	}
	// その他と衝突した場合
	else {

		// 何もしない
		return;
	}
}

void Player::Move() {

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState = {};

	// ゲームパッドが接続されている場合
	if (XInputGetState(0, &joyState) == ERROR_SUCCESS) {

		// 左スティックがデッドゾーンを超えていたら
		if (Input::GetInstance()->IsLeftThumbStickOutDeadZone()) {

			velocity_.x = (float)joyState.Gamepad.sThumbLX / SHRT_MAX * moveSpeedManual;
			velocity_.y = (float)joyState.Gamepad.sThumbLY / SHRT_MAX * moveSpeedManual;
		}
		else { // 左スティックがデッドゾーン内だったら

			// 速度を0にする
			velocity_.x = 0.0f;
			velocity_.y = 0.0f;
		}
	}
	else { // ゲームパッドが接続されていない場合

		bool w = Input::GetInstance()->PushKey(DIK_W); // Wキーが押されたか確認
		bool s = Input::GetInstance()->PushKey(DIK_S); // Sキーが押されたか確認
		bool a = Input::GetInstance()->PushKey(DIK_A); // Aキーが押されたか確認
		bool d = Input::GetInstance()->PushKey(DIK_D); // Dキーが押されたか確認

		if (w && !s) { // Wキーだけ押された場合

			// 速度を上向きに加算
			velocity_.y = moveSpeedManual;
		}
		else if (s && !w) { // Sキーだけ押された場合

			// 速度を下向きに加算
			velocity_.y = -moveSpeedManual;
		}
		else if (w && s) { // 同時に押されていた場合

			// 速度を0にする
			velocity_.y = 0.0f;
		}
		else { // WキーもSキーも押されていない場合

			// 速度を0にする
			velocity_.y = 0.0f;

		}

		if (a && !d) { // Aキーだけ押された場合

			// 速度を左向きに加算
			velocity_.x = -moveSpeedManual;
		}
		else if (d && !a) { // Dキーだけ押された場合

			// 速度を右向きに加算
			velocity_.x = moveSpeedManual;
		}
		else if (a && d) { // 同時に押されていた場合

			// 速度を0にする
			velocity_.x = 0.0f;
		}
		else { // AキーもDキーも押されていない場合

			// 速度を0にする
			velocity_.x = 0.0f;
		}
	}

	// 速度の長さを計算
	float length = Length(Vector2(velocity_.x, velocity_.y));

	// 長さが速さを超えていたら
	if (length > moveSpeedManual) {

		// 速度を正規化して速さを掛ける
		velocity_ = Normalize(velocity_) * moveSpeedManual;
	}

	// 速度をワールド変換に加算
	worldTransform_.AddTranslate(velocity_);
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
}

void Player::MoveToReticle() {

	// レティクルの位置を取得
	Vector3 reticlePos = reticle3D_->GetWorldTransform().GetWorldPosition();

	// レティクルの方向ベクトルを求める
	Vector3 toReticle = reticlePos - worldTransform_.GetWorldPosition();

	// 正規化
	toReticle = Normalize(toReticle);

	// 座標に速度を加算
	worldTransform_.AddTranslate(toReticle * moveSpeedManual);

	// 横軸の長さを求める
	float xzLength = Length(toReticle.x, toReticle.z);

	// ヨー(Y軸回りの回転)を求める
	float yaw = atan2f(toReticle.x, toReticle.z);

	// ピッチ(X軸回りの回転)を求める
	float pitch = atan2f(-toReticle.y, xzLength);

	// 回転を設定
	worldTransform_.SetRotate({ pitch, yaw, 0.0f });
}

void Player::AutoPilotInitialize() {
}

void Player::AutoPilotUpdate() {

	// Z方向のみの移動
	worldTransform_.AddTranslate({ 0.0f, 0.0f, moveSpeedAuto });
}

void Player::ManualInitialize() {

	// タイマーリセット
	fireTimer = 60.0f * 2.0f;
}

void Player::ManualUpdate() {

	// タイマーが0以下なら
	if (fireTimer <= 0) {

		// スペースキーが押されたら
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {

			Fire();

			// タイマーをリセット
			fireTimer = 60.0f * 0.2f;
		}
	}
	else {

		// タイマーをデクリメント
		fireTimer--;
	}

	// レティクルに向かって移動
	MoveToReticle();
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

	// パーティクルの初期化
	particleSetting.transform.scale = { 0.5f,0.5f,0.5f }; // スケール0.5f
	particleSetting.lifeTime = 2.0f;					  // 2秒
	particleSetting.useBillboard = false;				  // ビルボードを使わない
	particleSetting.randomizeRotate = true;
	particleSetting.randomizeVelocity = true;
	particleSetting.randomVelocityMin = { -2.0f, 0.0f, -2.0f };
	particleSetting.randomVelocityMax = { 2.0f, 2.0f, 2.0f };
	isParticleEmitted_ = false;

	// エミッターの生成
	particleEmitterRed = std::make_unique<ParticleEmitter>("Red", emitterTransform, 5, 0.0f, particleSetting);
	particleEmitterBlue = std::make_unique<ParticleEmitter>("Blue", emitterTransform, 25, 0.0f, particleSetting);
}

void Player::DeadUpdate() {

	// タイマーを進める
	deathTimer_ += 1.0f / 60.0f; // デルタタイム加算

	// 5秒経過したらタイトルシーンに戻る
	if (deathTimer_ >= 5.0f) {
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
	if (worldTransform_.GetWorldPosition().y <= kGroundHeight) {

		Vector3 Translate = { worldTransform_.GetWorldPosition().x, kGroundHeight, worldTransform_.GetWorldPosition().z };

		// Y座標を地面の高さに揃える
		worldTransform_.SetTranslate(Translate);

		// フラグを立てる
		isGroundHit_ = true;

		if (!isParticleEmitted_) {

			emitterTransform.translate = Translate;

			// エミッターの位置を設定
			particleEmitterRed->SetTransform(emitterTransform);
			particleEmitterBlue->SetTransform(emitterTransform);

			// パーティクルを発生させる
			particleEmitterRed->Emit();
			particleEmitterBlue->Emit();

			isParticleEmitted_ = true;
		}
	}
}

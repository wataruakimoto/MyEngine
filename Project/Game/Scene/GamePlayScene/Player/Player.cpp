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

	isDead = false;

	// コライダーの初期化
	Collider::Initialize();
	// コライダーにIDを設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIDDef::kPlayer));
}

void Player::Update() {

	// 将来的にはWSwitchから基底と継承先を分ける
	switch (mode_) {

	case PlayerMode::Title:

		// Z方向にしか移動できないようにする
		MoveToZ();

		break;

	case PlayerMode::Play:
	default:

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

	// 3Dオブジェクトの描画
	object->Draw();
}

void Player::Finalize() {
}

void Player::ShowImGui() {

#ifdef _DEBUG

	ImGui::Begin("Player");

	worldTransform_.ShowImGui();

	ImGui::SliderFloat3("Velocity", &velocity_.x, -0.2f, 0.2f);

	ImGui::Text("ScreenPos: (%.2f, %.2f)", screenPos_.x, screenPos_.y);

	object->ShowImGui();

	model->ShowImGui();

	ImGui::End();

#endif // _DEBUG
}

void Player::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が敵の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemy)) {
		// 死亡フラグを立てる
		isDead = true;
	}
	// 衝突相手が敵の弾の場合
	else if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemyBullet)) {

		// 死亡フラグを立てる
		isDead = true;
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

			velocity_.x = (float)joyState.Gamepad.sThumbLX / SHRT_MAX * moveSpeedPlay;
			velocity_.y = (float)joyState.Gamepad.sThumbLY / SHRT_MAX * moveSpeedPlay;
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
			velocity_.y = moveSpeedPlay;
		}
		else if (s && !w) { // Sキーだけ押された場合

			// 速度を下向きに加算
			velocity_.y = -moveSpeedPlay;
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
			velocity_.x = -moveSpeedPlay;
		}
		else if (d && !a) { // Dキーだけ押された場合

			// 速度を右向きに加算
			velocity_.x = moveSpeedPlay;
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
	if (length > moveSpeedPlay) {

		// 速度を正規化して速さを掛ける
		velocity_ = Normalize(velocity_) * moveSpeedPlay;
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
	worldTransform_.AddTranslate(toReticle * moveSpeedPlay);

	// 横軸の長さを求める
	float xzLength = Length(toReticle.x, toReticle.z);

	// ヨー(Y軸回りの回転)を求める
	float yaw = atan2f(toReticle.x, toReticle.z);

	// ピッチ(X軸回りの回転)を求める
	float pitch = atan2f(-toReticle.y, xzLength);

	// 回転を設定
	worldTransform_.SetRotate({ pitch, yaw, 0.0f });
}

void Player::MoveToZ() {

	worldTransform_.AddTranslate({ 0.0f, 0.0f, moveSpeedTitle });
}

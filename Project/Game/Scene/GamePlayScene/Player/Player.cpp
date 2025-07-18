#include "Player.h"
#include "input/input.h"
#include "2d/TextureManager.h"
#include "Scene/GamePlayScene/Collision/CollisionTypeIDDef.h"
#include "Scene/GamePlayScene/Reticle/Reticle3D.h"
#include "Math/MathVector.h"

#include <list>
#include <imgui.h>

void Player::Initialize() {

	// 基底クラスの初期化
	Basecharacter::Initialize();

	// モデルの生成・初期化
	model = new Model();
	model->Initialize("resources/player", "player.obj");

	// 3Dオブジェクトの生成・初期化
	object = new Object3d();
	object->Initialize();
	object->SetModel(model);
	object->SetScale({ 1.0f, 1.0f, 1.0f });

	isDead = false;

	// コライダーの初期化
	Collider::Initialize();
	// コライダーにIDを設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIDDef::kPlayer));
}

void Player::Update() {

	// デスフラグが立った弾を削除
	bullets.remove_if([](Bullet* bullet) {

		if (bullet->IsDead()) {

			bullet->Finalize();
			delete bullet;

			return true;
		}

		return false;
	});

	// 射撃
	if (fireTimer <= 0) {

		if (Input::GetInstance()->PushKey(DIK_SPACE)) {

			Fire();
			fireTimer = 60.0f * 0.2f;
		}
	}
	else {

		fireTimer--;
	}

	// 弾の更新
	for (Bullet* bullet : bullets) {

		bullet->ShowImGui();
		bullet->Update();
	}

	Move();

	// ワールド変換の更新
	worldTransform_.UpdateMatrix();

	object->SetTranslate(worldTransform_.GetTranslate());
	object->SetRotate(worldTransform_.GetRotate());

	// 3Dオブジェクトの更新
	object->Update();
}

void Player::Draw() {

	// 3Dオブジェクトの描画
	object->Draw();

	// 弾の描画
	for (Bullet* bullet : bullets) {

		bullet->Draw();
	}
}

void Player::Finalize() {

	// 残った弾の解放
	for (Bullet* bullet : bullets) {

		bullet->Finalize();
		delete bullet;
	}

	// 3Dオブジェクトの解放
	delete object;

	// モデルの解放
	delete model;
}

void Player::ShowImGui() {

#ifdef _DEBUG

	worldTransform_.ShowImGui("Player");

#endif // _DEBUG
}

void Player::Move() {

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState = {};

	// ゲームパッドが接続されているか確認
	if (XInputGetState(0, &joyState) == ERROR_SUCCESS) {

		// ゲームパッドが接続されている場合

		// ゲームパッド状態取得
		if (Input::GetInstance()->IsLeftThumbStickOutDeadZone()) {
			
			velocity_.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * moveSpeed;
			velocity_.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * moveSpeed;
		}
	}
	else {

		// ゲームパッドが接続されていない場合

		if (Input::GetInstance()->PushKey(DIK_W)) {

			velocity_.y += moveSpeed;
		}

		if (Input::GetInstance()->PushKey(DIK_S)) {

			velocity_.y -= moveSpeed;
		}

		if (Input::GetInstance()->PushKey(DIK_A)) {

			velocity_.x -= moveSpeed;
		}

		if (Input::GetInstance()->PushKey(DIK_D)) {

			velocity_.x += moveSpeed;
		}
	}

	// 速度をワールド変換に加算
	worldTransform_.AddTranslate(velocity_);
}

void Player::Fire() {

	// 弾の生成
	Bullet* bullet = new Bullet();
	bullet->Initialize();

	// 弾の初期位置をプレイヤーの位置に設定
	bullet->GetWorldTransform().SetTranslate(worldTransform_.GetTranslate());

	// 弾の初期速度を設定
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	velocity = reticle3D_->GetWorldTransform().GetTranslate() - worldTransform_.GetTranslate();
	bullet->SetVelocity(velocity);

	// 弾をリストに登録
	bullets.push_back(bullet);
}

void Player::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が敵の場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemy)) {
		// 死亡フラグを立てる
		isDead = true;
	}
	// その他と衝突した場合
	else {

		// 何もしない
		return;
	}
}
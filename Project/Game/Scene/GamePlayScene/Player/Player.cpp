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

	transform_.rotate = object->GetRotate();
	transform_.translate = object->GetTranslate();

	Move();

	object->SetTranslate(transform_.translate);
	object->SetRotate(transform_.rotate);

	// 3Dオブジェクトの更新
	object->Update();

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

	ImGui::Begin("Player");

	ImGui::DragFloat3("Scale", &transform_.scale.x, 0.1f);
	ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat("FireTimer", &fireTimer, 1.0f);

	ImGui::End();

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
			transform_.translate.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * moveSpeed;
			transform_.translate.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * moveSpeed;
		}
	}
	else {

		// ゲームパッドが接続されていない場合

		if (Input::GetInstance()->PushKey(DIK_W)) {

			transform_.translate.y += moveSpeed;
		}

		if (Input::GetInstance()->PushKey(DIK_S)) {

			transform_.translate.y -= moveSpeed;
		}

		if (Input::GetInstance()->PushKey(DIK_A)) {

			transform_.translate.x -= moveSpeed;
		}

		if (Input::GetInstance()->PushKey(DIK_D)) {

			transform_.translate.x += moveSpeed;
		}
	}
}

void Player::Fire() {

	// 弾の生成
	Bullet* bullet = new Bullet();
	bullet->Initialize();

	// 弾の初期位置をプレイヤーの位置に設定
	bullet->SetTranslate(transform_.translate);

	// 弾の初期速度を設定
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	velocity = reticle3D_->GetTranslate() - transform_.translate;
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
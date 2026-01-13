#include "Goal.h"
#include "Player/Player.h"
#include "Math/Easing.h"

#include <imgui.h>

using namespace Easing;

void Goal::Initialize() {

	/// ===== ゴールの初期化 ===== ///

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.SetTranslate({ 0.0f, 0.0f, 1700.0f });

	// モデルの生成・初期化
	model_ = std::make_unique<Model>();
	model_->Initialize("Goal", "Goal.obj");
	model_->SetColor({ 1.0f, 1.0f, 1.0f, 0.9f });

	// 3Dオブジェクトの生成・初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetScale({ 50.0f, 50.0f, 1.0f });

	/// ===== ゲートの初期化 ===== ///

	// ゲート用のワールド変換の初期化
	gateWorldTransform_.Initialize();
	gateWorldTransform_.SetTranslate({ 0.0f, 0.0f, 1650.0f });

	// ゲート用のモデルの生成・初期化
	gateModel_ = std::make_unique<Model>();
	gateModel_->Initialize("Gate", "Gate.obj");

	// ゲート用の3Dオブジェクトの生成・初期化
	gateObject_ = std::make_unique<Object3d>();
	gateObject_->Initialize();
	gateObject_->SetModel(gateModel_.get());
	gateObject_->SetScale({ 50.0f, 50.0f, 1.0f });

	alpha_ = 1.0f;
}

void Goal::Update() {

	/// ===== ゴールの更新 ===== ///

	// ワールド変換の更新
	worldTransform_.UpdateMatrix();

	object_->SetTranslate(worldTransform_.GetWorldPosition());
	object_->SetRotate(worldTransform_.GetRotate());

	// 3Dオブジェクトの更新
	object_->Update();

	/// ===== ゲートの更新 ===== ///

	/// === 移動の更新 === ///

	// 現在のゲートのY座標
	float gatePosY = gateWorldTransform_.GetWorldPosition().y;

	// プレイヤーからY座標を取得
	float playerPosY = player_->GetWorldTransform().GetWorldPosition().y;

	float followSpeed = 0.1f;

	// ゲートのY座標をプレイヤーのY座標に近づける
	gatePosY = Lerp(gatePosY, playerPosY, followSpeed);

	// ゲートのY座標をプレイヤーのY座標に合わせる
	gateWorldTransform_.SetTranslate({ 0.0f, gatePosY, 1650.0f });

	// ゲート用のワールド変換の更新
	gateWorldTransform_.UpdateMatrix();

	gateObject_->SetTranslate(gateWorldTransform_.GetWorldPosition());

	/// === 色の更新 === ///

	// ロックが解除されたら
	if (!isLocked) {

		// アルファ値を減少させる
		alpha_ -= 0.02f;

		// アルファ値の下限を0.0fに設定
		if (alpha_ < 0.0f) {

			alpha_ = 0.0f;
		}

		// ゲートのモデルの色を更新
		gateModel_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });
	}

	// ゲート用の3Dオブジェクトの更新
	gateObject_->Update();
}

void Goal::Draw() {

	// 3Dオブジェクトの描画
	object_->Draw();

	// アルファ値が0.0fより大きい場合
	if (alpha_ > 0.0f) {

		// ゲート用の3Dオブジェクトの描画
		gateObject_->Draw();
	}
}

void Goal::Finalize() {
}

void Goal::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("Goal");
	worldTransform_.ShowImGui();
	object_->ShowImGui();
	model_->ShowImGui();
	ImGui::End();

#endif // USE_IMGUI
}

void Goal::CheckGateCollision(int killCount) {

	// ロックが解除されていたら何もしない
	if (!isLocked) return;

	// 倒した数がノルマを超えていたら
	if (killCount >= kNormaCount) {

		// ロック解除
		isLocked = false;

		return;
	}

	/// ===== 壁の判定 ===== ///

	// ゲートの前面のZ座標
	const float hitLineZ = gateWorldTransform_.GetWorldPosition().z;

	// プレイヤーの座標
	Vector3 playerPos = player_->GetWorldTransform().GetWorldPosition();

	// プレイヤーがゲートの前を通過していたら
	if (playerPos.z >= hitLineZ) {

		// ゲート衝突時の処理を呼び出す
		player_->OnGateCollision();
	}
}

#include "Player.h"
#include "Input.h"
#include "GamePlayScene.h"
#include "Collision/CollisionTypeIDDef.h"
#include "MathVector.h"

#include "State/PlayerAutoState.h"
#include "State/PlayerManualState.h"

#include <imgui.h>

using namespace Engine;
using namespace MathVector;

/// ================================================== ///
/// コンストラクタ
Player::Player() {

	// 状態を一度だけ生成しておく
	states_[typeid(PlayerAutoState)] = std::make_unique<PlayerAutoState>();
	states_[typeid(PlayerManualState)] = std::make_unique<PlayerManualState>();
}

/// ================================================== ///
/// デストラクタ
Player::~Player() {
}

/// ================================================== ///
/// 初期化
void Player::Initialize() {

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.SetScale({ 2.0f,2.0f,2.0f });
	worldTransform_.SetTranslate({ 0.0f, 5.0f, 0.0f });

	// モデルの生成・初期化
	model = std::make_unique<Model>();
	model->Initialize("Player/player.obj");
	model->SetDiffuseSetting(2); // HalfLambert反射

	// 3Dオブジェクトの生成・初期化
	object = std::make_unique<Object3d>();
	object->Initialize();
	object->SetModel(model.get());
	object->GetWorldTransform().SetParent(&worldTransform_);

	// コライダーの生成
	collider_ = std::make_unique<Collider>(
		OBB{},
		static_cast<uint32_t>(CollisionTypeIDDef::kPlayer)
	);
	// コライダーの初期化
	collider_->Initialize();
	// コライダーに衝突時のコールバック関数を設定
	collider_->SetOnCollision([this](Collider* other) { OnCollision(other); });
	// コライダーにワールド変換を設定
	collider_->GetWorldTransform().SetParent(&worldTransform_);

	// 状態の初期化

	// オート操縦のコンテキストを設定
	AutoStateContext autoContext;
	autoContext.worldTransform = &worldTransform_;
	autoContext.moveSpeed = &moveSpeedAuto_;
	static_cast<PlayerAutoState*>(states_[typeid(PlayerAutoState)].get())->Initialize(autoContext);

	// マニュアル操縦のコンテキストを設定
	ManualStateContext manualContext;
	manualContext.worldTransform = &worldTransform_;
	manualContext.moveSpeed = &moveSpeedManual_;
	manualContext.camera = camera_;
	static_cast<PlayerManualState*>(states_[typeid(PlayerManualState)].get())->Initialize(manualContext);

	// 初期状態をオート操縦に設定
	ChangeState<PlayerAutoState>();
}

/// ================================================== ///
/// 更新
void Player::Update() {

	// 状態の変更がリクエストされていたら
	if (stateRequest_) {

		// 状態を変更
		state_ = stateRequest_.value();

		// 各状態の初期化を行う
		switch (state_) {

		case PlayerState::AutoPilot:

			ChangeState<PlayerAutoState>();

			break;

		case PlayerState::Manual:

			ChangeState<PlayerManualState>();

			break;
		}

		// リクエストをクリア
		stateRequest_ = std::nullopt;
	}

	if (currentState_) {

		// 現在の状態の更新
		currentState_->Update();
	}

	// ワールド変換の更新
	worldTransform_.Update();

	// コライダーの更新
	collider_->Update();

	// 3Dオブジェクトの更新
	object->Update();

	// ワールド座標からスクリーン座標に変換
	screenPos_ = ConvertWorldToScreen(worldTransform_.GetWorldPosition(), camera_->GetViewProjectionMatrix());
}

/// ================================================== ///
/// 描画
void Player::Draw() {

	// コライダーの描画
	collider_->Draw();

	// 3Dオブジェクトの描画
	object->Draw();
}

/// ================================================== ///
/// UIの描画
void Player::DrawUI() {

	// マニュアル状態のときのみUI描画
	auto* manualState = dynamic_cast<PlayerManualState*>(currentState_);

	if (manualState) {
		manualState->DrawUI();
	}
}

/// ================================================== ///
/// 終了
void Player::Finalize() {
}

void Player::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("Player");

	worldTransform_.ShowImGui();

	collider_->ShowImGui();

	ImGui::Text("ScreenPos: (%.2f, %.2f)", screenPos_.x, screenPos_.y);

	// 状態の表示
	ImGui::Text("State: %s",
		(state_ == PlayerState::AutoPilot) ? "AutoPilot" :
		(state_ == PlayerState::Manual) ? "Manual"
		: "Unknown");

	object->ShowImGui();

	model->ShowImGui();

	ImGui::End();

#endif // USE_IMGUI
}

/// ================================================== ///
/// 衝突時
void Player::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	bool isEnemy = typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemy);
	bool isEnemyBullet = typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemyBullet);
	bool isObstacle = typeID == static_cast<uint32_t>(CollisionTypeIDDef::kObstacle);

	if (!isEnemy && !isEnemyBullet && !isObstacle) return;

	// ManualState中のみ被弾処理を行う
	auto* manualState = dynamic_cast<PlayerManualState*>(currentState_);
	if (manualState) {
		manualState->OnHit();
	}
}

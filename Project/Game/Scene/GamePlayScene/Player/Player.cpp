#include "Player.h"
#include "Input.h"
#include "GamePlayScene.h"
#include "Collision/CollisionTypeIDDef.h"
#include "MathVector.h"

#include "State/PlayerAutoState.h"
#include "State/PlayerManualState.h"
#include "State/PlayerFallState.h"

#include <imgui.h>

using namespace Engine;
using namespace MathVector;

/// ================================================== ///
/// コンストラクタ
Player::Player() {

	// 状態を一度だけ生成しておく
	states_[typeid(PlayerAutoState)] = std::make_unique<PlayerAutoState>();
	states_[typeid(PlayerManualState)] = std::make_unique<PlayerManualState>();
	states_[typeid(PlayerFallState)] = std::make_unique<PlayerFallState>();
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
	worldTransform_.SetScale(defaultScale_);
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

	// 移動トレイル用パーティクルエミッターの生成・初期化
	moveEmitter_ = std::make_unique<ParticleEmitter>("PlayerMove", EmitterType::Interval, 1);
	moveEmitter_->Initialize();
	// プレイヤーに追従させる
	moveEmitter_->GetWorldTransform().SetParent(&worldTransform_);
	// プレイヤーの後方から発生させる
	moveEmitter_->SetTranslate({ 0.0f, 0.0f, -0.5f });
	// 最初は発生させない
	moveEmitter_->SetEmitting(false);

	// 着地パーティクル用エミッターの生成・初期化 (ワールド固定、プレイヤーには追従させない)
	deathEmitterBlue_ = std::make_unique<ParticleEmitter>("PlayerDeathBlue", EmitterType::OneShot, 40);
	deathEmitterBlue_->Initialize();

	// 状態の初期化

	// オート操縦のコンテキストを設定
	AutoStateContext autoContext;
	autoContext.worldTransform = &worldTransform_;
	autoContext.moveSpeed = &moveSpeedAuto_;
	autoContext.moveEmitter = moveEmitter_.get();
	static_cast<PlayerAutoState*>(states_[typeid(PlayerAutoState)].get())->Initialize(autoContext);

	// マニュアル操縦のコンテキストを設定
	ManualStateContext manualContext;
	manualContext.worldTransform = &worldTransform_;
	manualContext.camera = camera_;
	manualContext.defaultScale = defaultScale_;
	manualContext.moveEmitter = moveEmitter_.get();
	static_cast<PlayerManualState*>(states_[typeid(PlayerManualState)].get())->Initialize(manualContext);

	// 落下のコンテキストを設定
	FallStateContext fallContext;
	fallContext.worldTransform = &worldTransform_;
	fallContext.scale = defaultScale_;
	fallContext.deathEmitterBlue = deathEmitterBlue_.get();
	static_cast<PlayerFallState*>(states_[typeid(PlayerFallState)].get())->Initialize(fallContext);

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

		case PlayerState::Falling:

			ChangeState<PlayerFallState>();

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

	// 移動トレイル用パーティクルエミッターの更新
	moveEmitter_->Update();

	// 着地パーティクル用エミッターの更新
	deathEmitterBlue_->Update();

	// ワールド座標からスクリーン座標に変換
	screenPos_ = ConvertWorldToScreen(worldTransform_.GetWorldPosition(), camera_->GetViewProjectionMatrix());
}

/// ================================================== ///
/// 描画
void Player::Draw() {

	// コライダーの描画
	collider_->Draw();

	// マニュアル状態の取得
	auto* manualState = dynamic_cast<PlayerManualState*>(currentState_);

	// 落下状態の取得
	auto* fallState = dynamic_cast<PlayerFallState*>(currentState_);

	// マニュアル操縦状態のとき
	if (manualState) {

		// 点滅状態のとき
		if (manualState->IsVisible()) {

			// 3Dオブジェクトの描画
			object->Draw();
		}
	}
	// 落下状態で、かつ地面に着いたあとのとき
	else if (fallState && fallState->IsGroundHit()) {

		// 何も描画しない (着地パーティクルに切り替わる)
	}
	// それ以外の状態のとき
	else {

		// 3Dオブジェクトの描画
		object->Draw();
	}
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

	ImGui::DragFloat3("速度", &velocity_.x, 0.1f);

	ImGui::Text("ScreenPos: (%.2f, %.2f)", screenPos_.x, screenPos_.y);

	// 状態の表示
	ImGui::Text("State: %s",
		(state_ == PlayerState::AutoPilot) ? "AutoPilot" :
		(state_ == PlayerState::Manual) ? "Manual" :
		(state_ == PlayerState::Falling) ? "Falling"
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

		// 無敵中でなければ (=実際にダメージを受けるなら) シーンに通知
		bool wasInvincible = manualState->IsInvincible();

		manualState->OnHit();

		if (!wasInvincible && gamePlayScene_) {
			gamePlayScene_->OnPlayerDamaged();
		}
	}
}

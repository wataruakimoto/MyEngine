#include "Player.h"
#include "Input.h"
#include "GamePlayScene.h"
#include "Collision/CollisionTypeIDDef.h"
#include "Reticle/Reticle.h"
#include "LockOn/LockOn.h"
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

	// レティクルの生成
	reticle_ = std::make_unique<Reticle>();
	reticle_->Initialize();
	// レティクルのカメラ設定
	reticle_->SetCamera(camera_);

	// ロックオンの生成
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();
	// 自機をロックオンに設定
	lockOn_->SetPlayer(this);
	// カメラをロックオンに設定
	lockOn_->SetCamera(camera_);
	// 2Dレティクルをロックオンに設定
	lockOn_->SetReticle(reticle_.get());

	// 状態の初期化

	// オート操縦のコンテキストを設定
	AutoStateContext autoContext;
	autoContext.worldTransform = &worldTransform_;
	autoContext.moveSpeed = &moveSpeedAuto_;
	static_cast<PlayerAutoState*>(states_[typeid(PlayerAutoState)].get())->Initialize(autoContext);

	// マニュアル操縦のコンテキストを設定
	ManualStateContext manualContext;
	static_cast<PlayerManualState*>(states_[typeid(PlayerManualState)].get())->Initiaize(manualContext);

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

			ChangerState<PlayerAutoState>();

			break;

		case PlayerState::Manual:

			// マニュアルモードの初期化処理
			ManualInitialize();

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
	}

	// ワールド変換の更新
	worldTransform_.Update();

	// コライダーの更新
	collider_->Update();

	screenPos_ = ConvertWorldToScreen(worldTransform_.GetWorldPosition(), camera_->GetViewProjectionMatrix());

	// 3Dオブジェクトの更新
	object->Update();

	// エミッターの更新
	moveEmitter_->Update();
	particleEmitterRed->Update();
	particleEmitterBlue->Update();
}

/// ================================================== ///
/// 
void Player::Draw() {

	// コライダーの描画
	collider_->Draw();

	if (!isGroundHit_) {

		if (invincibleTimer_ > 0.0f) {
			if (static_cast<int>(invincibleTimer_) % 12 < 6) {
				return; // 描画処理をスキップ
			}
		}

		// 3Dオブジェクトの描画
		object->Draw();
	}

	// reticle_->Draw3D();
}

/// ================================================== ///
/// 
void Player::DrawUI() {

	reticle_->Draw2D();

	lockOn_->Draw();
}

void Player::Finalize() {
}

void Player::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("Player");

	worldTransform_.ShowImGui();

	collider_->ShowImGui();

	ImGui::Text("ScreenPos: (%.2f, %.2f)", screenPos_.x, screenPos_.y);

	ImGui::Checkbox("isDead", &isDead_);
	ImGui::Checkbox("isGroundHit", &isGroundHit_);

	// 状態の表示
	ImGui::Text("State: %s", (state_ == PlayerState::AutoPilot) ? "AutoPilot" :
		(state_ == PlayerState::Manual) ? "Manual" : "Unknown");

	object->ShowImGui();

	model->ShowImGui();

	ImGui::End();

	moveEmitter_->ShowImGui();

#endif // USE_IMGUI
}

/// ================================================== ///
/// 
void Player::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 状態がバレルロール中の場合
	if (isRolling_) {

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

		// 無敵時間中でなければダメージを受ける
		if (invincibleTimer_ <= 0.0f) {

			// 1ダメージを受ける
			DamageProcess(1);

			// 無敵タイマーをセット
			invincibleTimer_ = kInvincibleTime;
		}
	}
	// 衝突相手が敵の弾の場合
	else if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kEnemyBullet)) {

		// 無敵時間中でなければダメージを受ける
		if (invincibleTimer_ <= 0.0f) {

			// 1ダメージを受ける
			DamageProcess(1);

			// 無敵タイマーをセット
			invincibleTimer_ = kInvincibleTime;
		}
	}
	// 衝突相手が障害物の場合
	else if (typeID == static_cast<uint32_t>(CollisionTypeIDDef::kObstacle)) {

		// 無敵時間中でなければダメージを受ける
		if (invincibleTimer_ <= 0.0f) {

			// 1ダメージを受ける
			DamageProcess(1);

			// 無敵タイマーをセット
			invincibleTimer_ = kInvincibleTime;
		}
	}
	// その他と衝突した場合
	else {

		// 何もしない
		return;
	}
}

void Player::DamageProcess(uint16_t damage) {

	// HPが0より大きいなら
	if (hp_ > 0) {

		// ダメージ分HPを減らす
		hp_ -= damage;

		// シーンにダメージを通知
		if (gamePlayScene_) {
			gamePlayScene_->OnPlayerDamaged(hp_);
		}
	}
}

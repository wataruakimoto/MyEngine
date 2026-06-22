#define NOMINMAX

#include "Player.h"
#include "Input.h"
#include "GamePlayScene.h"
#include "Collision/CollisionTypeIDDef.h"
#include "MathVector.h"
#include "Easing.h"

#include "State/PlayerAutoState.h"

#include <algorithm>
#include <imgui.h>

using namespace Engine;
using namespace MathVector;
using namespace Easing;

/// ================================================== ///
/// コンストラクタ
Player::Player() {

	// 状態を一度だけ生成しておく
	states_[typeid(PlayerAutoState)] = std::make_unique<PlayerAutoState>();
}

/// ================================================== ///
/// デストラクタ
Player::~Player() {
}

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

	isDead_ = false;

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

	// コマンドの生成
	normalShotCommand_ = std::make_unique<NormalShotCommand>();
	lockOnAimCommand_ = std::make_unique<LockOnAimCommand>();
	lockOnShotCommand_ = std::make_unique<LockOnShotCommand>();

	hp_ = 5;

	// 移動パーティクルの生成
	moveEmitter_ = std::make_unique<Engine::ParticleEmitter>("PlayerMove", Engine::EmitterType::Interval, 1);
	// 移動パーティクルの初期化
	moveEmitter_->Initialize();
	// 移動パーティクルの位置をプレイヤーに設定
	moveEmitter_->GetWorldTransform().SetParent(&worldTransform_);
	// パーティクルを出さないようにする
	moveEmitter_->SetEmitting(false);

	// エミッターの生成
	particleEmitterRed = std::make_unique<ParticleEmitter>("PlayerDeathRed", EmitterType::OneShot, 10);
	particleEmitterRed->Initialize();
	particleEmitterRed->GetWorldTransform().SetParent(&worldTransform_);

	particleEmitterBlue = std::make_unique<ParticleEmitter>("PlayerDeathBlue", EmitterType::OneShot, 40);
	particleEmitterBlue->Initialize();
	particleEmitterBlue->GetWorldTransform().SetParent(&worldTransform_);
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

	ImGui::SliderFloat3("Velocity", &velocity_.x, -0.2f, 0.2f);

	ImGui::Text("ScreenPos: (%.2f, %.2f)", screenPos_.x, screenPos_.y);

	ImGui::Checkbox("isDead", &isDead_);
	ImGui::Checkbox("isGroundHit", &isGroundHit_);

	// 状態の表示
	ImGui::Text("State: %s", (state_ == PlayerState::AutoPilot) ? "AutoPilot" :
		(state_ == PlayerState::Manual) ? "Manual" : "Unknown");

	ImGui::Text("speedPlay: %.2f", moveSpeedManual);

	ImGui::Text("HP: %d", hp_);

	object->ShowImGui();

	model->ShowImGui();

	ImGui::End();

	moveEmitter_->ShowImGui();

#endif // USE_IMGUI
}

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

void Player::Fire(PlayerContext context) {

	// ロックオンモードなら
	if (isLockOnMode_) {

		// ロックオンショットコマンド実行
		lockOnShotCommand_->Execute(context);

	}
	// ロックオンモードでなければ
	else {

		// 通常射撃コマンド実行
		normalShotCommand_->Execute(context);
	}

	// 射撃間隔タイマーをリセット
	fireTimer_ = kFireDuration_;

	// 射撃アニメーション開始
	isFiring_ = true;
	fireAnimationTimer_ = kFireAnimationDuration_; // アニメーションタイマーをリセット
	object->GetWorldTransform().SetScale(fireScale_);
}

void Player::FireAnimationUpdate() {

	// デルタタイム分デクリメント
	fireAnimationTimer_ -= 1.0f / 60.0f;

	float t = 1.0f - (fireAnimationTimer_ / kFireAnimationDuration_); // 経過割合を計算
	float easedT = EaseOutCubic(t); // イージング適用
	Vector3 newScale = Lerp(fireScale_, defaultScale_, easedT); // スケールを補間

	// スケールを設定
	object->GetWorldTransform().SetScale(newScale);

	// タイマーが0以下になったら
	if (fireAnimationTimer_ <= 0.0f) {

		isFiring_ = false; // 射撃アニメーション終了
		newScale = defaultScale_; // スケールをデフォルトに戻す
	}
}

void Player::Rolling() {

	// ロール中フラグが立っていなければ終了
	if (!isRolling_) return;

	/// ===== タイマー処理 ===== ///

	// タイマーを進める
	rollTimer_ += 1.0f / 60.0f; // デルタタイム加算

	// 進行度を計算
	float t = rollTimer_ / rollDuration_;

	// イージング適用
	float easeT = EaseOutCubic(t);

	/// ===== 終了処理 ===== ///

	// タイマーが最大値に達したら
	if (t >= 1.0f) {

		// ロール完了
		t = 1.0f;

		// ロール中フラグを下ろす
		isRolling_ = false;

		// クールダウンタイマーをリセット
		rollCooldownTimer_ = kRollCooldownDuration_;

		// 以降の処理をスキップ
		return;
	}

	/// ===== 回転の計算 ===== ///

	// 現在の回転角度を計算
	float currentAngle = rollDirection_ * -1.0f * kMaxRollAngle_ * easeT;

	// 回転をZのみ設定
	Vector3 currentRotate = worldTransform_.GetRotate();
	currentRotate.z = currentAngle;
	worldTransform_.SetRotate(currentRotate);

	/// ===== 位置の計算 ===== ///

	// 1フレーム分の差分を求める
	float deltaT = easeT - preEaseT_; // 前回とのイージング値の差分

	// 移動量の計算
	Vector3 rollVelocity = { rollDirection_ * kMaxRollMove_ * deltaT, 0.0f, 0.0f };

	// 速度に加算
	velocity_ += rollVelocity;

	// 次回のために値を上書き
	preEaseT_ = easeT;
}

void Player::MoveToReticle() {

	// レティクルの位置を取得
	Vector3 reticlePos = reticle_->GetWorldTransform().GetWorldPosition();

	// レティクルの方向ベクトルを求める
	Vector3 toReticle = reticlePos - worldTransform_.GetWorldPosition();

	// 正規化
	toReticle = Normalize(toReticle);

	/// ===== 回転の処理 ===== ///

	// 横軸の長さを求める
	float xzLength = Length(toReticle.x, toReticle.z);

	// ヨー(Y軸回りの回転)を求める
	float yaw = atan2f(toReticle.x, toReticle.z);

	// ピッチ(X軸回りの回転)を求める
	float pitch = atan2f(-toReticle.y, xzLength);

	// 回転を取得
	Vector3 currentRotate = worldTransform_.GetRotate();

	// 回転させる
	currentRotate.x = pitch;
	currentRotate.y = yaw;

	// 回転を設定
	worldTransform_.SetRotate(currentRotate);

	/// ===== 位置の処理 ===== ///

	// 移動速度を計算
	Vector3 moveVelocity = toReticle * moveSpeedManual;

	// 速度を加算
	velocity_ += moveVelocity;

	float speed = Length(velocity_);

	// 速度が0より大きいなら
	if (speed > 0.0f) {

		float targetFrequency = baseFrequency_ / speed;

		// パーティクルの頻度を設定
		moveEmitter_->SetFrequency(baseFrequency_);

		// パーティクルを出す
		moveEmitter_->SetEmitting(true);
	}
	// 速度が0未満なら
	else {

		// パーティクルを出さない
		moveEmitter_->SetEmitting(false);
	}
}

void Player::ClampPosition() {

	// 大きさを取得
	Vector3 scale = worldTransform_.GetScale();

	// 加算後の座標を取得
	Vector3 currentPos = worldTransform_.GetTranslate();

	//// X軸のクランプ
	//currentPos.x = std::clamp(currentPos.x, kMoveMin.x + scale.x, kMoveMax.x - scale.x);
	//// Y軸のクランプ
	//currentPos.y = std::clamp(currentPos.y, kMoveMin.y + scale.y, kMoveMax.y - scale.y);

	// Y軸のクランプ(0以上)
	currentPos.y = std::max(currentPos.y, 0.0f + scale.y);

	// 原点からの距離を計算(XZ平面)
	float distanceFromOrigin = std::sqrt(currentPos.x * currentPos.x + currentPos.y * currentPos.y);

	// 半径25を超えている場合、円周上に制限
	const float kMaxRadius = 25.0f;
	if (distanceFromOrigin > kMaxRadius - scale.x) {
		float clampedRadius = kMaxRadius - scale.x;
		float ratio = clampedRadius / distanceFromOrigin;
		currentPos.x *= ratio;
		currentPos.y *= ratio;
	}

	// クランプ後の座標を設定
	worldTransform_.SetTranslate(currentPos);
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

void Player::ManualInitialize() {

	// 射撃のクールタイマーをリセット
	fireTimer_ = kFireDuration_;

	// バレルロールのクールタイマーをリセット
	rollCooldownTimer_ = kRollCooldownDuration_;
}

void Player::ManualUpdate() {

	// タイマー更新
	if (fireTimer_ > 0.0f) {
		fireTimer_ -= 1.0f / 60.0f;
	}

	// 速度をリセット
	velocity_ = { 0.0f, 0.0f, 0.0f };

	// コンテキスト作成
	PlayerContext context;
	context.player = this;
	context.reticle = reticle_.get();
	context.lockOn = lockOn_.get();
	context.objManager = objManager_;

	// 各キーの入力の状態
	bool isMouseLeftPush = Input::GetInstance()->PushMouseButton(MouseButton::Left); // 左クリック
	bool isMouseLeftRelease = Input::GetInstance()->ReleaseMouseButton(MouseButton::Left); // 左クリックリリース
	bool isAPush = Input::GetInstance()->PushKey('A'); // Aキー
	bool isDPush = Input::GetInstance()->PushKey('D'); // Dキー

	/// ===== 射撃処理 ===== ///

	//// 左クリックしている間
	//if (isMouseLeftPush) {
	//
	//	// 押されている時間を加算
	//	pressTimer_ += 1.0f / 60.0f;
	//
	//	// 一定時間以上押し続けたら
	//	if (pressTimer_ > kLockOnDuration_) {
	//
	//		// ロックオンモードへ移行
	//		isLockOnMode_ = true;
	//
	//		// ロックオンエイムコマンド実行
	//		lockOnAimCommand_->Execute(context);
	//	}
	//}
	//
	//// 左クリックを離したとき
	//if (isMouseLeftRelease) {
	//
	//	// タイマーが0以下なら
	//	if (fireTimer_ <= 0) {
	//
	//		// 射撃
	//		Fire(context);
	//	}
	//
	//	// 押下時間をリセット
	//	pressTimer_ = 0.0f;
	//
	//	// ロックオンモード解除
	//	isLockOnMode_ = false;
	//
	//	// ロックオンターゲットをクリア
	//	lockOn_->ClearTarget();
	//
	//}

	// 左クリックしている間
	if (isMouseLeftPush) {
		// タイマーが0以下なら
		if (fireTimer_ <= 0) {
			// 射撃
			Fire(context);
		}
	}

	if (isFiring_) {

		// 射撃アニメーション更新
		FireAnimationUpdate();
	}

	/// ===== バレルロール処理 ===== ///

	// タイマーが0以下かつロール中でなければ
	if (rollCooldownTimer_ <= 0.0f && !isRolling_) {

		// Aキーだけ押された場合
		if (isAPush && !isDPush) {

			rollDirection_ = -1; // 左回り

			rollTimer_ = 0.0f; // タイマーリセット

			preEaseT_ = 0.0f; // 

			isRolling_ = true; // ロール中フラグを立てる
		}
		// Dキーだけ押された場合
		else if (isDPush && !isAPush) {

			rollDirection_ = 1; // 右回り

			rollTimer_ = 0.0f; // タイマーリセット

			preEaseT_ = 0.0f; // 

			isRolling_ = true; // ロール中フラグを立てる
		}
	}
	else {

		// タイマーをデクリメント
		rollCooldownTimer_ -= 1.0f / 60.0f;
	}

	// バレルロール処理
	Rolling();

	/// ===== 移動処理 ===== ///

	// レティクルに向かって移動
	MoveToReticle();

	// 速度を加算
	worldTransform_.AddTranslate(velocity_);

	// 画面外に出ないように位置をクランプ
	ClampPosition();

	// レティクルの更新
	reticle_->Update();

	// ロックオンの更新
	lockOn_->Update();

	// 無敵タイマーの更新
	if (invincibleTimer_ > 0.0f) {
		invincibleTimer_ -= 1.0f;
	}

	// 移動パーティクルの発生フラグを立てる
	moveEmitter_->SetEmitting(true);
}

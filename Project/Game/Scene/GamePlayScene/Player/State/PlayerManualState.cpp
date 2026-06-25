#include "PlayerManualState.h"
#include "Input.h"

using namespace Engine;

/// ================================================== ///
/// 
void PlayerManualState::Initiaize(const ManualStateContext& context) {

	// 射撃のクールタイマーをリセット
	fireTimer_ = kFireDuration_;

	// バレルロールのクールタイマーをリセット
	rollCooldownTimer_ = kRollCooldownDuration_;

	// コマンドの生成
	normalShotCommand_ = std::make_unique<NormalShotCommand>();
	lockOnAimCommand_ = std::make_unique<LockOnAimCommand>();
	lockOnShotCommand_ = std::make_unique<LockOnShotCommand>();
}

/// ================================================== ///
/// 
void PlayerManualState::Update() {

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
	context_.worldTransform->AddTranslate(velocity_);

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

/// ================================================== ///
/// 
void PlayerManualState::Enter() {
}

/// ================================================== ///
/// 
void PlayerManualState::Exit() {
}

/// ================================================== ///
/// 射撃処理
void PlayerManualState::Fire(PlayerContext context) {

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

void PlayerManualState::FireAnimationUpdate() {

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

void PlayerManualState::Rolling() {

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

void PlayerManualState::MoveToReticle() {

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

void PlayerManualState::ClampPosition() {

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

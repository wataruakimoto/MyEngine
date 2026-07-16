#pragma once

#include "IPlayerState.h"
#include "WorldTransform.h"

/// === 前方宣言 === ///

namespace Engine {
	class ParticleEmitter;
}

/// <summary>
/// 落下状態のコンテキスト
/// </summary>
struct FallStateContext {
	Engine::WorldTransform* worldTransform = nullptr;		// ワールド変換
	Engine::Vector3 scale = { 1.0f, 1.0f, 1.0f };			// プレイヤーのスケール (地面判定オフセット用)
	Engine::ParticleEmitter* deathEmitterBlue = nullptr;	// 着地パーティクル (青)
};

/// <summary>
/// 落下状態 (時間切れなどでプレイヤーが墜落するときに使用)
/// </summary>
class PlayerFallState : public IPlayerState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="context">コンテキスト</param>
	void Initialize(const FallStateContext& context);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 状態に入ったときの処理
	/// </summary>
	void Enter() override;

	/// <summary>
	/// 状態から出るときの処理
	/// </summary>
	void Exit() override;

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
public:

	/// <summary>
	/// 地面に着いたかどうかを取得
	/// </summary>
	/// <returns></returns>
	bool IsGroundHit() const { return isGroundHit_; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// コンテキスト
	FallStateContext context_;

	/// ========== 落下用パラメータ ========== ///

	// 経過時間
	float fallTimer_ = 0.0f;

	// 落下速度
	Engine::Vector3 fallVelocity_ = { 0.0f, 0.0f, 0.0f };

	// 回転速度
	Engine::Vector3 fallRotateVelocity_ = { 0.0f, 0.0f, 0.0f };

	// 落下の初速
	const float kFallStartSpeed_ = -0.05f;

	// 落下の加速度 (プレイヤーの想定最高高度からでも数秒以内に着地するように、元の値より速くしてある)
	const float kFallAcceleration_ = -0.02f;

	// 落下の最大速度
	const float kMaxFallSpeed_ = -0.6f;

	// 回転の初速
	const float kRollSpeed_ = 0.1f;

	// 回転の加速度
	const float kRollAcceleration_ = 0.001f;

	// 横揺れの振幅
	const float kSwayAmplitude_ = 0.1f;

	// 横揺れの周波数
	const float kSwayFrequency_ = 4.0f;

	// 地面の高さ
	const float kGroundHeight_ = 0.0f;

	// 地面に着いたかどうか
	bool isGroundHit_ = false;

	// 着地パーティクルを発生させたかどうか
	bool isParticleEmitted_ = false;
};

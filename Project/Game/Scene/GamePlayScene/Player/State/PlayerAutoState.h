#pragma once

#include "IPlayerState.h"
#include "WorldTransform.h"

/// <summary>
/// オートパイロット状態のコンテキスト
/// </summary>
struct AutoStateContext {
	Engine::WorldTransform* worldTransform = nullptr; // ワールド変換
	float* moveSpeed = nullptr;						  // 移動の速さ
	bool* isAccelerating = nullptr;					  // 加速フラグ
	bool* isDecelerating = nullptr;					  // 減速フラグ
};

/// <summary>
/// オートパイロット状態
/// </summary>
class PlayerAutoState : public IPlayerState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="context">コンテキスト</param>
	void Initialize(const AutoStateContext& context);

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
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// 加速処理
	/// </summary>
	void Accelerate();

	/// <summary>
	/// 減速処理
	/// </summary>
	void Decelerate();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// コンテキスト
	AutoStateContext context_;

	// 加速の最大値
	const float kMaxMoveSpeed = 0.5f;

	// 減速の最小値
	const float kMinMoveSpeed = 0.0f;

	// 加速・減速の変化量
	const float acceleration_ = 0.01f;
};


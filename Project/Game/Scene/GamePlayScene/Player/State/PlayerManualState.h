#pragma once

#include "IPlayerState.h"
#include "PlayerCommand.h"
#include "WorldTransform.h"


/// <summary>
/// マニュアル操作状態のコンテキスト
/// </summary>
struct ManualStateContext {
	Engine::WorldTransform* worldTransform = nullptr; // ワールド変換
	float* moveSpeed = nullptr;						  // 移動の速さ
	bool* isLockOnMode = nullptr;					  // ロックオンモードフラグ
};

/// <summary>
/// マニュアル操作状態
/// </summary>
class PlayerManualState : public IPlayerState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="context">コンテキスト</param>
	void Initiaize(const ManualStateContext& context);

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


	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// コンテキスト
	ManualStateContext context_;

	/// ===== コマンド ===== ///

	std::unique_ptr<NormalShotCommand> normalShotCommand_ = nullptr; // 通常射撃コマンド

	std::unique_ptr<LockOnAimCommand> lockOnAimCommand_ = nullptr; // ロックオン照準コマンド

	std::unique_ptr<LockOnShotCommand> lockOnShotCommand_ = nullptr; // ロックオン射撃コマンド

	std::unique_ptr<BarrelRollCommand> barrelRollCommand_ = nullptr; // バレルロールコマンド
};


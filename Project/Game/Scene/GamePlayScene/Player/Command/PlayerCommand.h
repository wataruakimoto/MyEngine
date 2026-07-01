#pragma once

#include "IPlayerCommand.h"
#include "Vector3.h"

/// <summary>
/// 通常射撃コマンド
/// </summary>
class NormalShotCommand : public IPlayerCommand {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コマンド実行
	/// </summary>
	/// <param name="context">プレイヤーからの情報</param>
	void Execute(const PlayerContext& context) override;
};

/// <summary>
/// ロックオン照準コマンド
/// </summary>
class LockOnAimCommand : public IPlayerCommand {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コマンド実行
	/// </summary>
	/// <param name="context">プレイヤーからの情報</param>
	void Execute(const PlayerContext& context) override;
};

/// <summary>
/// ロックオン射撃コマンド
/// </summary>
class LockOnShotCommand : public IPlayerCommand {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コマンド実行
	/// </summary>
	/// <param name="context">プレイヤーからの情報</param>
	void Execute(const PlayerContext& context) override;
};

class BarrelRollCommand : public IPlayerCommand {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コマンド実行
	/// </summary>
	/// <param name="context">プレイヤーからの情報</param>
	void Execute(const PlayerContext& context) override;
};

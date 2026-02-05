#pragma once

#include "Vector3.h"

/// === 前方宣言 === ///
class Player;
class Reticle;
class LockOn;
class GamePlayScene;

// プレイヤーからの情報
struct PlayerContext {
	
	Player* player = nullptr;
	Reticle* reticle = nullptr;
	LockOn* lockOn = nullptr;
	GamePlayScene* scene = nullptr;
};

/// <summary>
/// プレイヤーコマンド基底クラス
/// </summary>
class IPlayerCommand {
	
///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IPlayerCommand() = default;

	/// <summary>
	/// コマンド実行
	/// </summary>
	/// <param name="context">プレイヤーからの情報</param>
	virtual void Execute(const PlayerContext& context) = 0;
};

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

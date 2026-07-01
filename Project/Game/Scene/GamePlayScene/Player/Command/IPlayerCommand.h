#pragma once

/// === 前方宣言 === ///
class Player;
class Reticle;
class LockOn;
class GameObjectManager;

// プレイヤーからの情報
struct PlayerContext {

	Player* player = nullptr;
	Reticle* reticle = nullptr;
	LockOn* lockOn = nullptr;
	GameObjectManager* objManager = nullptr;
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

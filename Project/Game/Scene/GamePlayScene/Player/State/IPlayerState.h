#pragma once

#include "../Player.h"

/// <summary>
/// プレイヤーの状態基底クラス
/// </summary>
class IPlayerState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IPlayerState() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 状態に入ったときの処理
	/// </summary>
	virtual void Enter() = 0;

	/// <summary>
	/// 状態から出るときの処理
	/// </summary>
	virtual void Exit() = 0;

	/// ================================================== ///
	/// セッター
	/// ================================================== ///
public:

	/// <summary>
	/// コンテキストのセッター
	/// </summary>
	/// <param name="context">コンテキスト</param>
	void SetContext(const Player::StateContext& context) { context_ = context; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
protected:

	Player::StateContext context_;
};
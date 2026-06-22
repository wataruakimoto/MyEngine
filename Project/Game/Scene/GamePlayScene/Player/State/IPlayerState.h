#pragma once

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
};
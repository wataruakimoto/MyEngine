#pragma once

/// === 前方宣言 === ///

class GamePlayScene;

/// <summary>
/// プレイシーンの状態を表す基底クラス
/// </summary>
class IPlayState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IPlayState() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scene"></param>
	virtual void Initialize(GamePlayScene* scene) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
public:

	/// <summary>
	/// 状態の終了フラグを取得
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return isFinished_; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
protected:

	// 状態の終了フラグ
	bool isFinished_ = false;

	// シーンの借りポインタ
	GamePlayScene* scene_ = nullptr;
};
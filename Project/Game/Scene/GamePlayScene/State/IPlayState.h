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

	virtual ~IPlayState() = default;

	virtual void Initialize(GamePlayScene* scene) = 0;

	virtual void Update() = 0;

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
public:

	bool IsFinished() const { return isFinished_; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
protected:

	bool isFinished_ = false;

	// シーンの借りポインタ
	GamePlayScene* scene_ = nullptr;
};
#pragma once

/// === 前方宣言 === ///

class GamePlayScene;

/// <summary>
/// ゲームルールクラス
/// </summary>
class GameRule {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
public:

	/// <summary>
	/// クリア判定
	/// </summary>
	/// <returns></returns>
	bool IsClear() const { return isClear_; }

	/// <summary>
	/// ゲームオーバー判定
	/// </summary>
	/// <returns></returns>
	bool IsGameOver() const { return isGameOver_; }

	/// ================================================== ///
	/// セッター
	/// ================================================== ///
public:

	/// <summary>
	/// シーンのセッター
	/// </summary>
	/// <param name="scene">シーン</param>
	void SetPlayScene(GamePlayScene* scene) { scene_ = scene; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// クリアフラグ
	bool isClear_ = false;

	// ゲームオーバーフラグ
	bool isGameOver_ = false;

	/// ===== 借りポインタ・インスタンス ===== ///

	// シーンの借りポインタ
	GamePlayScene* scene_ = nullptr;
};


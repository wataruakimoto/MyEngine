#pragma once

/// ===== シーンマネージャ前方宣言 ===== ///
class SceneManager;

/// ===== シーン基底 ===== ///
class BaseScene {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// シーンマネージャのポインタ
	SceneManager* sceneManager_ = nullptr;
};
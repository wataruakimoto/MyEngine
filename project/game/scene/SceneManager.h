#pragma once

#include "BaseScene.h"
#include "AbstractSceneFactory.h"

/// ===== シーン管理 ===== ///
class SceneManager {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static SceneManager* instance;

	// コンストラクタの隠蔽
	SceneManager() = default;
	// デストラクタの隠蔽
	~SceneManager() = default;
	// コピーコンストラクタの封印
	SceneManager(SceneManager&) = delete;
	// コピー代入演算子の封印
	SceneManager& operator=(SceneManager&) = delete;

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static SceneManager* GetInstance();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// シーンファクトリーのセッター
	/// </summary>
	/// <param name="sceneFactory">シーンファクトリー</param>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

	/// <summary>
	/// シーンを変更
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void ChangeScene(const std::string& sceneName);

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 今のシーン
	BaseScene* scene_ = nullptr;

	// 次のシーン
	BaseScene* nextScene_ = nullptr;

	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
};
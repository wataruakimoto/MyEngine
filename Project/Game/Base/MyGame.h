#pragma once

#include "Framework.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///
	class TextureManager;
	class ModelManager;
}

/// === ゲーム関連 === ///
class MyGame : public Engine::Framework{

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// 全リソースの読み込み
	/// </summary>
	void LoadAllResource();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// エディットモードフラグ
#ifdef _DEBUG
	bool isEditMode_ = true;
#else
	bool isEditMode_ = false;
#endif

	/// ===== インスタンス ===== ///

	// テクスチャマネージャのインスタンス
	Engine::TextureManager* textureManager = nullptr;

	// モデルマネージャのインスタンス
	Engine::ModelManager* modelManager = nullptr;
};
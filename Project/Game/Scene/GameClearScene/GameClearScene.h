#pragma once

#include "BaseScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/SpriteCommon.h"
#include "Sprite/Sprite.h"

#include <memory>

/// ===== ゲームクリアシーン ===== ///
class GameClearScene : public BaseScene {

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

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui() override;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// シーンマネージャのインスタンス
	SceneManager* sceneManager_ = SceneManager::GetInstance();

	// インプットのインスタンス
	Input* input_ = Input::GetInstance();

	// テクスチャマネージャのインスタンス
	TextureManager* textureManager_ = TextureManager::GetInstance();

	// スプライトコモンのインスタンス
	SpriteCommon* spriteCommon_ = SpriteCommon::GetInstance();

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;
};
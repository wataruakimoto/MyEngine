#pragma once

#include "BaseScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/SpriteCommon.h"
#include "Sprite/Sprite.h"

#include <memory>

/// ===== ゲームオーバーシーン ===== ///
class GameOverScene : public BaseScene {

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
	/// フィルター適応のある描画
	/// </summary>
	void DrawFiltered() override;

	/// <summary>
	/// フィルター適応のない描画
	/// </summary>
	void DrawUnfiltered() override;

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
	Engine::SceneManager* sceneManager_ = Engine::SceneManager::GetInstance();

	// インプットのインスタンス
	Engine::Input* input_ = Engine::Input::GetInstance();

	// テクスチャマネージャのインスタンス
	Engine::TextureManager* textureManager_ = Engine::TextureManager::GetInstance();

	// スプライトコモンのインスタンス
	Engine::SpriteCommon* spriteCommon_ = Engine::SpriteCommon::GetInstance();

	// スプライト
	std::unique_ptr<Engine::Sprite> sprite_ = nullptr;
};
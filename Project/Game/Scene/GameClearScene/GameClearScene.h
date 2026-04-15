#pragma once

#include "BaseScene.h"
#include "Camera.h"
#include "CameraControll/ICameraController.h"
#include "Floor/Floor.h"
#include "Cylinder/Cylinder.h"
#include "Player/Player.h"
#include "Light/LightManager.h"
#include "Sprite/Sprite.h"

#include <memory>
#include <optional>
#include <string>

/// ===== 前方宣言 ===== ///

namespace Engine {

	class Object3dRenderer;
	class FilterManager;
	class FogFilter;
	class TransitionManager;
	class SceneManager;
	class Input;
	class SpriteRenderer;
}

/// ===== ゲームクリアシーン ===== ///
class GameClearScene : public BaseScene {

private:

	enum class GameClearFlowState {
		FadeOut,   // フェードアウト
		WaitInput, // 入力待ち
		FadeIn,    // フェードイン
	};

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

private:

	void FadeOutInitialize();

	void FadeOutUpdate();

	void WaitInputInitialize();

	void WaitInputUpdate();

	void FadeInInitialize();

	void FadeInUpdate();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// カメラ
	std::unique_ptr<Engine::Camera> camera_ = nullptr;

	// カメラコントローラ
	std::unique_ptr<ICameraController> cameraController_ = nullptr;

	// 床
	std::unique_ptr<Floor> floor_ = nullptr;

	// シリンダー
	std::unique_ptr<Cylinder> cylinder_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// プレイヤーの移動速度
	float playerMoveSpeed_ = 0.0f;

	// 状態
	GameClearFlowState gameClearFlowState_ = GameClearFlowState::WaitInput;

	// 状態リクエスト
	std::optional<GameClearFlowState> stateRequest_ = std::nullopt;

	// 次のシーン
	std::string nextScene_;

	std::unique_ptr<Engine::Sprite> text_ = nullptr;

	std::unique_ptr<Engine::Sprite> guide_ = nullptr;

	/// ========== シングルトン・借りポインタ ========== ///

	// オブジェクトレンダラーのインスタンス
	Engine::Object3dRenderer* object3dRenderer_ = nullptr;

	// ライトマネージャのポインタ
	std::unique_ptr<Engine::LightManager> lightManager_ = nullptr;

	// フィルターマネージャのインスタンス
	Engine::FilterManager* filterManager_ = nullptr;

	// フォグの借りポインタ
	Engine::FogFilter* fogFilter_ = nullptr;

	// 遷移マネージャのポインタ
	Engine::TransitionManager* transitionManager = nullptr;

	// シーンマネージャのインスタンス
	Engine::SceneManager* sceneManager_ = nullptr;

	// インプットのインスタンス
	Engine::Input* input_ = nullptr;

	// スプライトレンダラーのインスタンス
	Engine::SpriteRenderer* spriteRenderer_ = nullptr;
};
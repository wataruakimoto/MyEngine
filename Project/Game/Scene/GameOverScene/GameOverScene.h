#pragma once

#include "BaseScene.h"
#include "Camera.h"
#include "CameraControll/ICameraController.h"
#include "Floor/Floor.h"
#include "Cylinder/Cylinder.h"
#include "Player/Player.h"
#include "Sprite/Sprite.h"

#include <memory>
#include <optional>
#include <string>

/// ===== 前方宣言 ===== ///

namespace Engine {

	class Object3dRenderer;
	class FilterManager;
	class Input;
	class SceneManager;
	class TransitionManager;
	class FogFilter;
	class RadialBlurFilter;

	class SpriteRenderer;
}

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

private:

	void FadeOutInitialize();

	void FadeOutUpdate();

	void WaitInputInitialize();

	void WaitInputUpdate();

	void SpeedUpInitialize();

	void SpeedUpUpdate();

	void FadeInInitialize();

	void FadeInUpdate();

private:

	enum class GameOverFlowState {
		
		FadeOut,   // フェードアウト
		WaitInput, // 入力待ち
		SpeedUp,   // 自機を加速させる
		FadeIn,    // フェードイン
	};

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

	// ブラーの中心点
	Engine::Vector2 blurCenter_ = { 0.5f, 0.5f };

	// ブラーの強さ
	float blurStrength_ = 0.0f;

	// ブラーの最大値
	const float kMaxBlurStrength = 0.1f;

	// フェードの色
	Engine::Vector3 fadeColor_ = {};

	// 次のシーン
	std::string nextScene_;

	// 状態
	GameOverFlowState gameOverFlowState_ = GameOverFlowState::WaitInput;

	// 状態リクエスト
	std::optional<GameOverFlowState> stateRequest_ = std::nullopt;

	std::unique_ptr<Engine::Sprite> text_ = nullptr;

	/// ===== インスタンス・借りポインタ ===== ///

	// オブジェクトレンダラーのインスタンス
	Engine::Object3dRenderer* object3dRenderer_ = nullptr;

	// フィルターマネージャのインスタンス
	Engine::FilterManager* filterManager_ = nullptr;

	// インプットのインスタンス
	Engine::Input* input_ = nullptr;

	// シーンマネージャのインスタンス
	Engine::SceneManager* sceneManager_ = nullptr;

	// 遷移マネージャのポインタ
	Engine::TransitionManager* transitionManager = nullptr;

	// フォグの借りポインタ
	Engine::FogFilter* fogFilter_ = nullptr;

	// ラジアルブラーの借りポインタ
	Engine::RadialBlurFilter* radialBlurFilter_ = nullptr;

	// スプライトレンダラーのインスタンス
	Engine::SpriteRenderer* spriteRenderer_ = nullptr;
};
#pragma once

#include "BaseScene.h"
#include "SceneManager.h"
#include "OffscreenRendering/FilterManager.h"
#include "Input.h"
#include "Camera.h"
#include "CameraControll/ICameraController.h"
#include "Player/Player.h"
#include "Floor/Floor.h"
#include "Cylinder/Cylinder.h"
#include "SkyBox/SkyBoxGame.h"
#include "UI/TitleUI.h"
#include "UI/StartUI.h"
#include "UI/BlackScreen.h"

#include <memory>
#include <optional>

class RadialBlurFilter;

// タイトルの流れの状態
enum class TitleFlowState {
    Blackout,	// 画面が真っ黒
    SlideIn,	// UIスライドイン
    FadeOut,	// 画面フェードアウト
    WaitInput,	// 入力待ち
    MoveUp,		// UI上に移動
    SpeedUp, 	// 自機を加速させる
};

/// ===== タイトルシーン ===== ///
class TitleScene : public BaseScene {

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
/// クラス内関数
///-------------------------------------------///
private:

    // 各状態の初期化と更新

    void BlackoutInitialize();

    void BlackoutUpdate();

    void SlideInInitialize();

    void SlideInUpdate();

    void FadeOutInitialize();

    void FadeOutUpdate();

    void WaitInputInitialize();

    void WaitInputUpdate();

    void MoveUpInitialize();

    void MoveUpUpdate();

    void SpeedUpInitialize();

    void SpeedUpUpdate();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

    // シーンマネージャのインスタンス
    SceneManager* sceneManager_ = SceneManager::GetInstance();

    // フィルターマネージャのインスタンス
    FilterManager* filterManager_ = FilterManager::GetInstance();

    // 入力のインスタンス
    Input* input_ = Input::GetInstance();

    // カメラ
    std::unique_ptr<Camera> camera_ = nullptr;

    // カメラコントローラ
    std::unique_ptr<ICameraController> cameraController_ = nullptr;

    // プレイヤー
    std::unique_ptr<Player> player_ = nullptr;

    // 床
    std::unique_ptr<Floor> floor_ = nullptr;

    // シリンダー
    std::unique_ptr<Cylinder> cylinder_ = nullptr;

    // 天球
    std::unique_ptr<SkyBoxGame> skyBox_ = nullptr;

    // 黒画面UI
    std::unique_ptr<BlackScreen> blackScreen_ = nullptr;

    // タイトルUI
    std::unique_ptr<TitleUI> titleUI_ = nullptr;

    // スタートUI
    std::unique_ptr<StartUI> startUI_ = nullptr;

    // タイトルの流れの状態
    TitleFlowState titleFlowState_ = TitleFlowState::Blackout;

    // 状態リクエスト
    std::optional<TitleFlowState> stateRequest_ = std::nullopt;

    // ラジアルブラー借りポインタ
    RadialBlurFilter* radialBlurFilter_ = nullptr;

    // ブラーの中心座標
    Vector2 blurCenter_ = { 0.5f, 0.5f };

    // ブラーの強さ
    float blurStrength_ = 0.0f;

    // ブラーの最大値
    const float kMaxBlurStrength = 0.07f;

    // プレイヤー移動の最大値
	const float kMaxPlayerMoveSpeed = 5.0f;

    // プレイヤーの移動速度
	float playerMoveSpeed_ = 0.0f;
};
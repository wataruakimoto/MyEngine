#pragma once

#include "IPlayState.h"
#include "UI/PauseUI.h"

#include <memory>

/// === 前方宣言 === ///

namespace Engine {
	class FilterManager;
	class VignetteFilter;
	class TransitionManager;
}

/// <summary>
/// ポーズ状態
/// </summary>
class PauseState : public IPlayState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scene">ゲームプレイシーンのポインタ</param>
	void Initialize(GamePlayScene* scene) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// ポーズUIのインスタンス
	std::unique_ptr<PauseUI> pauseUI = nullptr;

	bool isSelectedRestart_ = false;

	// フィルターマネージャのインスタンス
	Engine::FilterManager* filterManager_ = nullptr;

	// ビネットフィルター借りポインタ
	Engine::VignetteFilter* vignetteFilter_ = nullptr;

	// 遷移マネージャのインスタンス
	Engine::TransitionManager* transitionManager_ = nullptr;
};
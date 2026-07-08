#pragma once

#include "IPlayState.h"
#include "UI/ResultUI.h"

#include <memory>

/// === 前方宣言 === ///

namespace Engine {
	class FilterManager;
	class VignetteFilter;
	class TransitionManager;
}

class GameObjectManager;
class GameRule;
class Player;
class Goal;

/// <summary>
/// エンディング状態
/// </summary>
class EndingState : public IPlayState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scene"></param>
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
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// フェードインを開始する
	/// </summary>
	void StartFadeIn();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:
		
	/// ========== フェードイン用 ========== ///

	// フェードイン開始済みフラグ
	bool isFadeStarted_ = false;

	// UIアニメーション完了フラグ
	bool isAnimationFinished_ = false;

	// 遷移先シーン名
	std::string nextScene_;

	/// ===== 借りポインタ・インスタンス ===== ///

	// オブジェクトマネージャのインスタンス
	GameObjectManager* gameObjectManager_ = nullptr;
	
	// ゲームルールの借りポインタ
	GameRule* gameRule_ = nullptr;

	// プレイヤーの借りポインタ
	Player* player_ = nullptr;

	// ゴールの借りポインタ
	Goal* goal_ = nullptr;

	// リザルトUI
	std::unique_ptr<ResultUI> resultUI_ = nullptr;

	// フィルターマネージャのインスタンス
	Engine::FilterManager* filterManager_ = nullptr;

	// ビネットフィルター借りポインタ
	Engine::VignetteFilter* vignetteFilter_ = nullptr;

	// 遷移マネージャのインスタンス
	Engine::TransitionManager* transitionManager_ = nullptr;
};
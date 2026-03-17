#pragma once

#include "BaseTransition.h"
#include "Vector4.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///

	class FilterManager;
	class FadeFilter;
}

/// <summary>
/// フェード遷移クラス
/// </summary>
class FadeTransition : public Engine::BaseTransition {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="color">色</param>
	FadeTransition(Engine::Vector4 color);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FadeTransition() override = default;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="progress"></param>
	void Update(float progress) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// フェードの色
	Engine::Vector4 fadeColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	// フィルターマネージャのインスタンス
	Engine::FilterManager* filterManager = nullptr;

	// フェードフィルターのポインタ
	Engine::FadeFilter* fadeFilter = nullptr;
};


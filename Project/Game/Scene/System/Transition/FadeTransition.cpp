#include "FadeTransition.h"
#include "OffscreenRendering/FilterManager.h"
#include "OffscreenRendering/Filters/FadeFilter.h"

using namespace Engine;

FadeTransition::FadeTransition(Vector4 color) {

	// フィルターマネージャのインスタンスを取得
	filterManager = Engine::FilterManager::GetInstance();

	// フェードフィルターのポインタを取得
	fadeFilter = filterManager->GetFadeFilter();

	// フェードの色を設定
	fadeColor = color;
	fadeColor.w = 0.0f; // 初期のアルファ値は0 (完全に透明)

	// フェードフィルターの色を設定
	fadeFilter->SetColor(fadeColor);
	fadeFilter->SetIsActive(true);
}

void FadeTransition::Update(float progress) {

	// フェードの進行度に応じてアルファ値を更新
	fadeColor.w = progress;

	// フェードフィルターの色を更新
	fadeFilter->SetColor(fadeColor);
}

void FadeTransition::Draw() {
}

#include "FilterManager.h"
#include "Filters/BaseFilter.h"
#include "SceneBuffer.h"
#include "PostProcessBuffer.h"

#include <imgui.h>

using namespace Engine;

void FilterManager::Initialize() {

	// 各種フィルターの生成・初期化・登録

	fullScreenFilter_ = std::make_unique<FullScreenFilter>();
	fullScreenFilter_->Initialize();
	filters_["FullScreen"] = std::move(fullScreenFilter_);
	filterOrder.push_back("FullScreen");

	grayscaleFilter_ = std::make_unique<GrayscaleFilter>();
	grayscaleFilter_->Initialize();
	filters_["Grayscale"] = std::move(grayscaleFilter_);
	filterOrder.push_back("Grayscale");

	vignetteFilter_ = std::make_unique<VignetteFilter>();
	vignetteFilter_->Initialize();
	filters_["Vignette"] = std::move(vignetteFilter_);
	filterOrder.push_back("Vignette");

	boxBlurFilter_ = std::make_unique<BoxBlurFilter>();
	boxBlurFilter_->Initialize();
	filters_["BoxBlur"] = std::move(boxBlurFilter_);
	filterOrder.push_back("BoxBlur");

	gaussianBlurFilter_ = std::make_unique<GaussianBlurFilter>();
	gaussianBlurFilter_->Initialize();
	filters_["GaussianBlur"] = std::move(gaussianBlurFilter_);
	filterOrder.push_back("GaussianBlur");

	luminanceOutlineFilter_ = std::make_unique<LuminanceOutlineFilter>();
	luminanceOutlineFilter_->Initialize();
	filters_["LuminanceOutline"] = std::move(luminanceOutlineFilter_);
	filterOrder.push_back("LuminanceOutline");

	depthOutlineFilter_ = std::make_unique<DepthOutlineFilter>();
	depthOutlineFilter_->Initialize();
	filters_["DepthOutline"] = std::move(depthOutlineFilter_);
	filterOrder.push_back("DepthOutline");

	radialBlurFilter_ = std::make_unique<RadialBlurFilter>();
	radialBlurFilter_->Initialize();
	filters_["RadialBlur"] = std::move(radialBlurFilter_);
	filterOrder.push_back("RadialBlur");

	dissolveFilter_ = std::make_unique<DissolveFilter>();
	dissolveFilter_->Initialize();
	// マスクテクスチャのファイルパスを設定
	dissolveFilter_->SetMaskTextureFilePath("noise0.png");
	filters_["Dissolve"] = std::move(dissolveFilter_);
	filterOrder.push_back("Dissolve");

	randomFilter_ = std::make_unique<RandomFilter>();
	randomFilter_->Initialize();
	filters_["Random"] = std::move(randomFilter_);
	filterOrder.push_back("Random");

	fogFilter_ = std::make_unique<FogFilter>();
	fogFilter_->Initialize();
	filters_["Fog"] = std::move(fogFilter_);
	filterOrder.push_back("Fog");

	/// ===== 描画順序の設定 ===== ///

	// 一旦クリア
	filterOrder.clear();

	filterOrder = {

		// エッジ検出系
		"Dissolve",
		"DepthOutline",
		"LuminanceOutline",

		// 色調補正やノイズ
		"Grayscale",
		"Random",

		// ブラー系
		"BoxBlur",
		"GaussianBlur",
		"RadialBlur",

		// その他
		"Vignette",
		"Fog",

		"FullScreen" // 最後にフルスクリーンフィルターでシーンに書き戻す
	};
}

void FilterManager::Draw(SceneBuffer* scene, PostProcessBuffer* postProcess) {

	/// ===== 1つもフィルターが有効じゃなかったら ===== ///

	// 有効なフィルターがあるかどうかのフラグ
	bool isAnyFilterActive = false;

	// フィルターを順番にチェック
	for (const auto& key : filterOrder) {

		// 有効なフィルターが見つかったら
		if (filters_[key]->GetIsActive()) {

			// 有効なフィルターがあった
			isAnyFilterActive = true;

			// チェック終了
			break;
		}
	}

	// 有効なフィルターが一つもなかったら終了
	if (!isAnyFilterActive) return;

	/// ===== 描画準備 ===== ///

	// それぞれのSRVインデックスを取得
	uint32_t currentSrvIndex = scene->GetSrvIndex();    // 現在のSRVインデックス
	uint32_t depthSrvIndex = scene->GetDepthSrvIndex(); // 深度用SRVインデックス

	// シーンバッファからの結果かどうか
	bool isResultFromScene = true;

	/// ===== 全フィルターを回す ===== ///

	for(const auto& key : filterOrder) {
		
		// フィルターが無効ならスキップ
		if (!filters_[key]->GetIsActive()) continue;

		// シーンバッファからの結果なら
		if (isResultFromScene) {

			// 描画前処理
			postProcess->PreDraw();

			// SRVインデックスをセット (シーンのはず)
			filters_[key]->SetSrvIndex(currentSrvIndex);

			// 深度アウトラインフィルターかフォグのとき
			if (key == "DepthOutline") {

				// 深度用SRVインデックスをセット
				auto* outline = static_cast<DepthOutlineFilter*>(filters_[key].get());
				outline->SetDepthSrvIndex(depthSrvIndex);
				outline->SetCamera(camera);
			}
			else if (key == "Fog") {

				auto* fog = static_cast<FogFilter*>(filters_[key].get());
				fog->SetDepthSrvIndex(depthSrvIndex);
				fog->SetCamera(camera);
			}

			// 描画
			filters_[key]->Draw();

			// 描画後処理
			postProcess->PostDraw();

			// 次はポストプロセスバッファからになる
			currentSrvIndex = postProcess->GetSrvIndex();
			isResultFromScene = false;
		}
		// ポストプロセスバッファからの結果なら
		else {

			// 描画前処理
			scene->PreDrawResolve();

			// SRVインデックスをセット (ポストプロセスのはず)
			filters_[key]->SetSrvIndex(currentSrvIndex);

			// 描画
			filters_[key]->Draw();

			// 描画後処理
			scene->PostDraw();

			// 次はシーンバッファからになる
			currentSrvIndex = scene->GetSrvIndex();
			isResultFromScene = true;
		}
	}

	/// ===== シーンにコピー ===== ///

	// 最後の結果がシーンバッファからでなければ
	if (!isResultFromScene) {

		// 描画前処理
		scene->PreDrawResolve();

		// フルスクリーンフィルターのSRVインデックスをセット (ポストプロセスのはず)
		filters_["FullScreen"]->SetSrvIndex(currentSrvIndex);

		// 描画
		filters_["FullScreen"]->Draw();

		// 描画後処理
		scene->PostDraw();
	}
}

void FilterManager::DrawTexture(uint32_t srvIndex) {

	// "FullScreen" フィルターが存在するか確認
	if (filters_.find("FullScreen") != filters_.end()) {

		// 指定された画像をセット
		filters_["FullScreen"]->SetSrvIndex(srvIndex);

		// 描画実行 (今のRTVに向かって描かれます)
		filters_["FullScreen"]->Draw();
	}
}

void FilterManager::Finalize() {

	// フィルターを全て解放
	//filters_.clear();

	// インスタンスの解放
	delete instance;
	instance = nullptr;
}

void FilterManager::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("Filter Manager");

	for (const auto& key : filterOrder) {

		filters_[key]->ShowImGui();
	}

	ImGui::End();

#endif // USE_IMGUI
}

FilterManager* FilterManager::instance = nullptr;

FilterManager* FilterManager::GetInstance() {

	if (instance == nullptr) {
		instance = new FilterManager();
	}
	return instance;
}
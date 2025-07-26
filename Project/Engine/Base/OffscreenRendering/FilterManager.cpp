#include "FilterManager.h"
#include "Filters/BaseFilter.h"
#include "PostEffect.h"

#include <imgui.h>

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
	dissolveFilter_->SetMaskTextureFilePath("Resources", "noise0.png");
	filters_["Dissolve"] = std::move(dissolveFilter_);
	filterOrder.push_back("Dissolve");

	randomFilter_ = std::make_unique<RandomFilter>();
	randomFilter_->Initialize();
	filters_["Random"] = std::move(randomFilter_);
	filterOrder.push_back("Random");

	// SRVインデックスをPostEffectから取得
	uint32_t srvIndex = postEffect->GetSRVIndex();
	uint32_t depthSrvIndex = postEffect->GetDepthSRVIndex();

	// フィルターのPostEffectのセット
	for (const auto& key : filterOrder) {

		filters_[key]->SetSrvIndex(srvIndex);

		// 深度アウトラインフィルターのとき
		if (key == "DepthOutline") {

			// 深度用SRVインデックスをセット
			static_cast<DepthOutlineFilter*>(filters_[key].get())->SetDepthSrvIndex(depthSrvIndex);
		}
	}
}

void FilterManager::Draw() {

	for (const auto& key : filterOrder) {

		// 深度アウトラインフィルターのとき
		if (key == "DepthOutline") {

			// カメラのセット
			static_cast<DepthOutlineFilter*>(filters_[key].get())->SetCamera(camera);
		}

		// フィルターがアクティブでない場合は
		if (!filters_[key]->GetIsActive()) {

			// スキップ
			continue;
		}

		filters_[key]->Draw();
	}
}

void FilterManager::Finalize() {

	// フィルターを全て解放
	filters_.clear();

	// インスタンスの解放
	delete instance;
	instance = nullptr;
}

void FilterManager::ShowImGui() {

	ImGui::Begin("Filter Manager");

	for (const auto& key : filterOrder) {

		filters_[key]->ShowImGui();
	}

	ImGui::End();
}

FilterManager* FilterManager::instance = nullptr;

FilterManager* FilterManager::GetInstance() {

	if (instance == nullptr) {
		instance = new FilterManager();
	}
	return instance;
}
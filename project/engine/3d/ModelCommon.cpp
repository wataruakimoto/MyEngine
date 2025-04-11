#include "Modelcommon.h"
#include "base/DirectXUtility.h"
#include "base/SwapChain.h"

ModelCommon* ModelCommon::instance = nullptr;

ModelCommon* ModelCommon::GetInstance() {

	if (instance == nullptr) {
		instance = new ModelCommon;
	}
	return instance;
}

void ModelCommon::Initialize(DirectXUtility* dxUtility, SwapChain* swapChain) {

	// 引数をメンバ変数にコピー
	dxUtility_ = dxUtility;
	swapChain_ = swapChain;
}

void ModelCommon::Finalize() {
	delete instance;
	instance = nullptr;
}
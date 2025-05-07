#include "Modelcommon.h"
#include "base/DirectXUtility.h"

ModelCommon* ModelCommon::instance = nullptr;

ModelCommon* ModelCommon::GetInstance() {

	if (instance == nullptr) {
		instance = new ModelCommon;
	}
	return instance;
}

void ModelCommon::Initialize(DirectXUtility* dxUtility) {

	// 引数をメンバ変数にコピー
	dxUtility_ = dxUtility;
}

void ModelCommon::Finalize() {
	delete instance;
	instance = nullptr;
}
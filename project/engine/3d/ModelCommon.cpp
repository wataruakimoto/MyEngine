#include "Modelcommon.h"

ModelCommon* ModelCommon::instance = nullptr;

ModelCommon* ModelCommon::GetInstance() {

	if (instance == nullptr) {
		instance = new ModelCommon;
	}
	return instance;
}

void ModelCommon::Initialize(DirectXCommon* dxCommon) {

	// 引数をメンバ変数にコピー
	dxCommon_ = dxCommon;
}

void ModelCommon::Finalize() {
	delete instance;
	instance = nullptr;
}
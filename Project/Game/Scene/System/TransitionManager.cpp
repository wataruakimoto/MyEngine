#include "TransitionManager.h"

TransitionManager* TransitionManager::instance = nullptr;

void TransitionManager::Finalize() {

	// インスタンス破棄
	delete instance;

	// ヌル代入
	instance = nullptr;
}

TransitionManager* TransitionManager::GetInstance() {
    
	if (instance == nullptr) {
		instance = new TransitionManager;
	}

	return instance;
}

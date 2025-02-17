#include "ParticleManager.h"

ParticleManager* ParticleManager::instance = nullptr;

ParticleManager* ParticleManager::GetInstance() {
	
	if (instance == nullptr) {
		instance = new ParticleManager;
	}
	return instance;
}

void ParticleManager::Initialize() {
}

void ParticleManager::Finalize() {

	delete instance;
	instance = nullptr;
}
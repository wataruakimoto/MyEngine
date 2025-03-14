#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include <imgui.h>

ParticleEmitter::ParticleEmitter(const std::string& name, const Transform& transform, uint32_t count, float frequency) {

	// 名前
	this->particleName = name;

	// 位置
	this->transform = transform;

	// 発生数
	this->count = count;

	// 発生頻度
	this->frequency = frequency;

	// 頻度用時刻
	this->frequencyTime = 0.0f;
}

void ParticleEmitter::Emit() {

	// 時間を進める
	frequencyTime += kDeltaTime;

	// 頻度より大きいなら発生
	if (frequency <= frequencyTime) {

		// 発生処理
		ParticleSystem::GetInstance()->Emit(particleName, transform.translate, count);

		// 余計に過ぎた時間も加味して頻度計算する
		frequencyTime -= frequency;
	}
}

void ParticleEmitter::ShowImGui(const char* name) {

	ImGui::Begin(name);

	ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f);
	ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f);
	ImGui::DragFloat3("Position", &transform.translate.x, 0.01f);

	ImGui::DragFloat("Frequency", &frequency, 0.1f);
	ImGui::DragFloat("FrequencyTime", &frequencyTime, 0.1f);

	// ボタンを押したらパーティクル発生
	if (ImGui::Button("Emit")) {
		ParticleSystem::GetInstance()->Emit(particleName, transform.translate, count);
	}
	
	ImGui::End();
}

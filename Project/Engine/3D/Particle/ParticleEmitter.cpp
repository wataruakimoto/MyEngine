#include "ParticleEmitter.h"
#include "ParticleSystem.h"

#include <imgui.h>

ParticleEmitter::ParticleEmitter(const std::string& name, const Transform& transform, uint32_t count, float frequency, Particle setting) {

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

	// 設定項目
	this->settings = setting;
}

void ParticleEmitter::Emit() {

	// 時間を進める
	frequencyTime += kDeltaTime;

	// 頻度より大きいなら発生
	if (frequency <= frequencyTime) {

		// 発生処理
		ParticleSystem::GetInstance()->Emit(particleName, transform.translate, count, settings);

		// 余計に過ぎた時間も加味して頻度計算する
		frequencyTime -= frequency;
	}
}

void ParticleEmitter::ShowImGui(const char* name) {

#ifdef USE_IMGUI
	ImGui::Begin(name);

	if (ImGui::TreeNode("Emitter")) {

		ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f);
		ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f);
		ImGui::DragFloat3("Position", &transform.translate.x, 0.01f);

		ImGui::DragInt("Count", reinterpret_cast<int*>(&count), 1.0f, 1);

		ImGui::DragFloat("Frequency", &frequency, 0.1f);
		ImGui::DragFloat("FrequencyTime", &frequencyTime, 0.1f);

		// ボタンを押したらパーティクル発生
		if (ImGui::Button("Emit")) {
			ParticleSystem::GetInstance()->Emit(particleName, transform.translate, count, settings);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Setting")) {

		// 設定項目
		ImGui::DragFloat3("Scale", &settings.transform.scale.x, 0.01f);
		ImGui::DragFloat3("Rotate", &settings.transform.rotate.x, 0.01f);
		ImGui::DragFloat3("Position", &settings.transform.translate.x, 0.01f);
		ImGui::DragFloat3("Velocity", &settings.velocity.x, 0.01f);
		ImGui::ColorEdit4("Color", &settings.color.x);
		ImGui::DragFloat("LifeTime", &settings.lifeTime, 0.1f);
		ImGui::Checkbox("UseLifeTime", &settings.useLifeTime);
		ImGui::Checkbox("UseBillboard", &settings.useBillboard);

		// ランダム
		if (ImGui::TreeNode("Randomize")) {

			if (ImGui::TreeNode("Scale")) {
				ImGui::Checkbox("Randomize", &settings.randomizeScale);
				ImGui::DragFloat3("Min", &settings.randomScaleMin.x, 0.01f);
				ImGui::DragFloat3("Max", &settings.randomScaleMax.x, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Rotate")) {
				ImGui::Checkbox("Randomize", &settings.randomizeRotate);
				ImGui::DragFloat3("Min", &settings.randomRotateMin.x, 0.01f);
				ImGui::DragFloat3("Max", &settings.randomRotateMax.x, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Translate")) {
				ImGui::Checkbox("Randomize", &settings.randomizeTranslate);
				ImGui::DragFloat3("Min", &settings.randomTranslateMin.x, 0.01f);
				ImGui::DragFloat3("Max", &settings.randomTranslateMax.x, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Velocity")) {
				ImGui::Checkbox("Randomize", &settings.randomizeVelocity);
				ImGui::DragFloat3("Min", &settings.randomVelocityMin.x, 0.01f);
				ImGui::DragFloat3("Max", &settings.randomVelocityMax.x, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Color")) {
				ImGui::Checkbox("Randomize", &settings.randomizeColor);
				ImGui::DragFloat4("Min", &settings.randomColorMin.x, 0.01f);
				ImGui::DragFloat4("Max", &settings.randomColorMax.x, 0.01f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("LifeTime")) {
				ImGui::Checkbox("Randomize", &settings.randomizeLifeTime);
				ImGui::DragFloat("Min", &settings.randomLifeTimeMin, 0.1f);
				ImGui::DragFloat("Max", &settings.randomLifeTimeMax, 0.1f);
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	ImGui::End();

#endif // USE_IMGUI
}

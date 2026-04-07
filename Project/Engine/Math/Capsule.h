#pragma once

#include "Vector3.h"

#include <imgui.h>

namespace Engine {

	/// <summary>
	/// カプセル
	/// </summary>
	struct Capsule {
		Vector3 start; // 始点
		Vector3 end;   // 終点
		float radius; // 半径
	};

	inline void ShowImGuiCapsule(const char* label, Capsule& capsule) {

#ifdef USE_IMGUI

		ImGui::Begin(label);

		ImGui::DragFloat3("始点", &capsule.start.x, 0.01f);

		ImGui::DragFloat3("終点", &capsule.end.x, 0.01f);

		ImGui::DragFloat("半径", &capsule.radius, 0.01f);

		ImGui::End();

#endif // USE_IMGUI
	}

	inline void ShowImGuiCapsuleTree(const char* label, Capsule& capsule) {

#ifdef USE_IMGUI

		if (ImGui::TreeNodeEx(label)) {

			ImGui::DragFloat3("始点", &capsule.start.x, 0.01f);

			ImGui::DragFloat3("終点", &capsule.end.x, 0.01f);

			ImGui::DragFloat("半径", &capsule.radius, 0.01f);

			ImGui::TreePop();
		}

#endif // USE_IMGUI
	}
}
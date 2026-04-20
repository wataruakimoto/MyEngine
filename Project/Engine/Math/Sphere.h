#pragma once

#include "Vector3.h"

#include <imgui.h>

namespace Engine {

	/// <summary>
	/// 球
	/// </summary>
	struct Sphere {
		Vector3 center; // 中心座標
		float radius; // 半径
	};

	inline void ShowImGuiSphere(const char* label, Sphere& sphere) {

#ifdef USE_IMGUI

		ImGui::Begin(label);

		ImGui::DragFloat3("中心座標", &sphere.center.x, 0.01f);

		ImGui::DragFloat("半径", &sphere.radius, 0.01f);

		ImGui::End();

#endif // USE_IMGUI
	}

	inline void ShowImGuiSphereTree(const char* label, Sphere& sphere) {

#ifdef USE_IMGUI

		if (ImGui::TreeNodeEx(label)) {

			ImGui::DragFloat3("中心座標", &sphere.center.x, 0.01f);

			ImGui::DragFloat("半径", &sphere.radius, 0.01f);

			ImGui::TreePop();
		}

#endif // USE_IMGUI
	}
}
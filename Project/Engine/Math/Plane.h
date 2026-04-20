#pragma once

#include "Vector3.h"

#include <imgui.h>

namespace Engine {

	/// <summary>
	/// 平面
	/// </summary>
	struct Plane {
		Vector3 normal; // 法線
		float distance; // 距離
	};

	inline void ShowImGuiPlane(const char* label, Plane& plane) {

#ifdef USE_IMGUI

		ImGui::Begin(label);

		ImGui::DragFloat3("法線", &plane.normal.x, 0.01f);

		ImGui::DragFloat("距離", &plane.distance, 0.01f);

		ImGui::End();

#endif // USE_IMGUI
	}

	inline void ShowImGuiPlaneTree(const char* label, Plane& plane) {

#ifdef USE_IMGUI

		if (ImGui::TreeNodeEx(label)) {

			ImGui::DragFloat3("法線", &plane.normal.x, 0.01f);

			ImGui::DragFloat("距離", &plane.distance, 0.01f);

			ImGui::TreePop();
		}

#endif // USE_IMGUI
	}
}
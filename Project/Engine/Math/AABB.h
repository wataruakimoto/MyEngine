#pragma once

#include "Vector3.h"

#include <imgui.h>

namespace Engine {

	/// <summary>
	/// 軸平行境界ボックス (Axis-Aligned Bounding Box)
	/// </summary>
	struct AABB {
		Vector3 min; // 最小座標
		Vector3 max; // 最大座標
	};

	inline void ShowImGuiAABB(const char* label, AABB& aabb) {

#ifdef USE_IMGUI

		ImGui::Begin(label);

		ImGui::DragFloat3("最小点", &aabb.min.x, 0.01f);

		ImGui::DragFloat3("最大点", &aabb.max.x, 0.01f);

		ImGui::End();

#endif // USE_IMGUI
	}

	inline void ShowImGuiAABBTree(const char* label, AABB& aabb) {

#ifdef USE_IMGUI

		if (ImGui::TreeNodeEx(label)) {

			ImGui::DragFloat3("最小点", &aabb.min.x, 0.01f);

			ImGui::DragFloat3("最大点", &aabb.max.x, 0.01f);

			ImGui::TreePop();
		}

#endif // USE_IMGUI
	}
}
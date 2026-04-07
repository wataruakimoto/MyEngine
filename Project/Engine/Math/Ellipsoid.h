#pragma once

#include "Vector3.h"

#include <imgui.h>

namespace Engine {

	/// <summary>
	/// 楕円形
	/// </summary>
	struct Ellipsoid {
		Vector3 center; // 中心座標
		Vector3 radius; // 半径
	};

	inline void ShowImGuiEllipsoid(const char* label, Ellipsoid& ellipsoid) {

#ifdef USE_IMGUI

		ImGui::Begin(label);

		ImGui::DragFloat3("中心座標", &ellipsoid.center.x, 0.01f);

		ImGui::DragFloat3("半径", &ellipsoid.radius.x, 0.01f);

		ImGui::End();

#endif // USE_IMGUI
	}

	inline void ShowImGuiEllipsoidTree(const char* label, Ellipsoid& ellipsoid) {

#ifdef USE_IMGUI

		if (ImGui::TreeNodeEx(label)) {

			ImGui::DragFloat3("中心座標", &ellipsoid.center.x, 0.01f);

			ImGui::DragFloat3("半径", &ellipsoid.radius.x, 0.01f);

			ImGui::TreePop();
		}

#endif // USE_IMGUI
	}
}
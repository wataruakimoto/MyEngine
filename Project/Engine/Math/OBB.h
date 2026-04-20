#pragma once

#include "Vector3.h"

#include <imgui.h>

namespace Engine {

	/// <summary>
	/// 有効境界箱 (Oriented Bounding Box)
	/// </summary>
	struct OBB {
		Vector3 center; // 中心座標
		Vector3 orientations[3]; // 各軸の方向ベクトル
		Vector3 halfSize; // 中心から面までの距離 (座標軸方向の半分の長さ)
	};

	inline void ShowImGuiOBB(const char* label, OBB& obb) {

#ifdef USE_IMGUI

		ImGui::Begin(label);

		ImGui::DragFloat3("中心座標", &obb.center.x, 0.01f);

		if (ImGui::TreeNodeEx("方向ベクトル")) {

			ImGui::DragFloat3("右 : X", &obb.orientations[0].x, 0.01f);
			ImGui::DragFloat3("上 : Y", &obb.orientations[1].x, 0.01f);
			ImGui::DragFloat3("前 : Z", &obb.orientations[2].x, 0.01f);

			ImGui::TreePop();
		}

		ImGui::DragFloat3("半分の長さ", &obb.halfSize.x, 0.01f);

		ImGui::End();

#endif // USE_IMGUI
	}

	inline void ShowImGuiOBBTree(const char* label, OBB& obb) {

#ifdef USE_IMGUI

		if (ImGui::TreeNodeEx(label)) {

			ImGui::DragFloat3("中心座標", &obb.center.x, 0.01f);

			if (ImGui::TreeNodeEx("方向ベクトル")) {

				ImGui::DragFloat3("右 : X", &obb.orientations[0].x, 0.01f);
				ImGui::DragFloat3("上 : Y", &obb.orientations[1].x, 0.01f);
				ImGui::DragFloat3("前 : Z", &obb.orientations[2].x, 0.01f);

				ImGui::TreePop();
			}

			ImGui::DragFloat3("半分の長さ", &obb.halfSize.x, 0.01f);

			ImGui::TreePop();
		}

#endif // USE_IMGUI
	}
}
#pragma once

#include <imgui.h>

namespace Engine {

	struct Matrix4x4 {
		float m[4][4];
	};

	inline void ShowImGuiMatrix4x4(const char* label, Matrix4x4& matrix) {

#ifdef USE_IMGUI

		ImGui::Begin(label);

		ImGui::BeginDisabled(); // これ以降のUIを操作不可にする

		ImGui::DragFloat4("##row0", matrix.m[0]);
		ImGui::DragFloat4("##row1", matrix.m[1]);
		ImGui::DragFloat4("##row2", matrix.m[2]);
		ImGui::DragFloat4("##row3", matrix.m[3]);

		ImGui::EndDisabled();   // 操作不可を解除

		ImGui::End();

#endif // USE_IMGUI
	}

	inline void ShowImGuiMatrix4x4Tree(const char* label, Matrix4x4& matrix) {

#ifdef USE_IMGUI

		if (ImGui::TreeNodeEx(label)) {

			ImGui::BeginDisabled(); // これ以降のUIを操作不可にする

			ImGui::DragFloat4("##row0", matrix.m[0]);
			ImGui::DragFloat4("##row1", matrix.m[1]);
			ImGui::DragFloat4("##row2", matrix.m[2]);
			ImGui::DragFloat4("##row3", matrix.m[3]);

			ImGui::EndDisabled();   // 操作不可を解除

			ImGui::TreePop();
		}

#endif // USE_IMGUI
	}
}
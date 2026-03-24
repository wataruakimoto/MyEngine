#include "WorldTransform.h"
#include "MathVector.h"
#include "MathMatrix.h"

#include <imgui.h>

using namespace Engine;
using namespace MathVector;
using namespace MathMatrix;

void WorldTransform::Initialize() {

	scale_ = { 1.0f, 1.0f, 1.0f };

	rotate_ = { 0.0f, 0.0f, 0.0f };

	translate_ = { 0.0f, 0.0f, 0.0f };

	worldMatrix_ = MakeIdentity4x4();
}

void WorldTransform::Update() {

	// ワールド行列を作成
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);

	// 親が割り当てられていたら
	if (parent_) {

		// 親のワールド行列を掛け合わせる
		worldMatrix_ *= parent_->worldMatrix_;
	}
}

void WorldTransform::ShowImGui() {

#ifdef USE_IMGUI

	// ツリーで表示
	if (ImGui::TreeNodeEx("ワールド座標変換", ImGuiTreeNodeFlags_Framed)) {

		// 各種値を表示
		ImGui::DragFloat3("拡大縮小", &scale_.x, 0.1f);
		ImGui::DragFloat3("回転", &rotate_.x, 0.01f);
		ImGui::DragFloat3("平行移動", &translate_.x, 0.1f);

		// ワールド行列を表示
		if (ImGui::TreeNodeEx("ワールド行列", ImGuiTreeNodeFlags_DefaultOpen)) {

			if (ImGui::BeginTable("MatrixTable", 4, ImGuiTableFlags_Borders)) {

				for (int row = 0; row < 4; ++row) {

					ImGui::TableNextRow();

					for (int col = 0; col < 4; ++col) {

						ImGui::TableSetColumnIndex(col);

						ImGui::Text("%.2f", worldMatrix_.m[row][col]);
					}
				}

				ImGui::EndTable();
			}
			
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

#endif // USE_IMGUI
}

void WorldTransform::AddRotate(const Vector3& value) {

	rotate_ += value;
}

void WorldTransform::AddTranslate(const Vector3& value) {

	translate_ += value;
}

Vector3 WorldTransform::GetWorldPosition() const {

	Vector3 worldPosition = {
		worldMatrix_.m[3][0],
		worldMatrix_.m[3][1],
		worldMatrix_.m[3][2]
	};

	return worldPosition;
}

#include "WorldTransform.h"
#include "Math/MathVector.h"
#include "Math/MathMatrix.h"

#include <imgui.h>

using namespace MathMatrix;

void WorldTransform::Initialize() {

	// 初期値を設定
	scale_ = { 1.0f, 1.0f, 1.0f };
	rotate_ = { 0.0f, 0.0f, 0.0f };
	translate_ = { 0.0f, 0.0f, 0.0f };

	// ワールド行列を単位行列に設定
	worldMatrix_ = MakeIdentity4x4();

	// 親はnullptrで初期化
	parent_ = nullptr;
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

	// ツリーで表示
	if (ImGui::TreeNode("WorldTransform")) {

		// 各種値を表示
		ImGui::DragFloat3("Scale", &scale_.x, 0.1f);
		ImGui::DragFloat3("Rotate", &rotate_.x, 0.01f);
		ImGui::DragFloat3("Translate", &translate_.x, 0.1f);

		// ワールド行列を表示
		if (ImGui::TreeNode("WorldMatrix")) {

			for (int i = 0; i < 4; ++i) {
				
				ImGui::Text("%f, %f, %f, %f", worldMatrix_.m[i][0], worldMatrix_.m[i][1], worldMatrix_.m[i][2], worldMatrix_.m[i][3]);
			}
			
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void WorldTransform::AddTranslate(const Vector3& value) {

	translate_ += value;
}

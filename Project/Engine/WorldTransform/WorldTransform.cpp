#include "WorldTransform.h"
#include "DirectXUtility.h"
#include "Camera.h"
#include "MathVector.h"
#include "MathMatrix.h"

#include <imgui.h>

using namespace MathVector;
using namespace MathMatrix;

void WorldTransform::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility_ = DirectXUtility::GetInstance();

	// リソース作成
	transformationResource_ = dxUtility_->CreateBufferResource(sizeof(Transformation));

	// リソースにデータを書き込むためのアドレスを取得、割り当てする
	transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData_));

	// 座標変換データの初期化
	transformationData_.worldMatrix = MakeIdentity4x4(); // 単位行列を書き込む
	transformationData_.WVPMatrix = MakeIdentity4x4(); // 単位行列を書き込む
	transformationData_.worldInverseTranspose = MakeIdentity4x4(); // 単位行列を書き込む
}

void WorldTransform::UpdateMatrix() {

	// ワールド行列を作成
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);

	// 親が割り当てられていたら
	if (parent_) {

		// 親のワールド行列を掛け合わせる
		worldMatrix_ *= parent_->worldMatrix_;
	}

	// カメラが割り当てられていたら
	if (camera_) {

		// ビュープロジェクション行列を取得
		Matrix4x4 viewProjectionMatrix = camera_->GetViewProjectionMatrix();

		// ワールド行列とビュープロジェクション行列を掛け合わせてWVP行列を作成
		WVPMatrix_ = worldMatrix_ * viewProjectionMatrix;
	}
	else {

		// WVP行列はワールド行列と同じにしておく
		WVPMatrix_ = worldMatrix_;
	}

	// 座標変換データに書き込む
	transformationData_.worldMatrix = worldMatrix_;
	transformationData_.WVPMatrix = WVPMatrix_;
	transformationData_.worldInverseTranspose = Inverse(worldMatrix_);
}

void WorldTransform::TransferMatrix() {

	// コマンドリストにリソースを転送
	dxUtility_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationResource_->GetGPUVirtualAddress());
}

void WorldTransform::Update() {

	// 行列の更新
	UpdateMatrix();

	// 行列の転送
	TransferMatrix();
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

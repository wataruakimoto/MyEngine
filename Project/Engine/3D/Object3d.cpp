#include "Object3d.h"
#include "Object3dCommon.h"
#include "math/MathVector.h"
#include "math/MathMatrix.h"
#include "winApp/WinApp.h"
#include "base/DirectXUtility.h"
#include "2d/TextureManager.h"
#include "ModelManager.h"
#include "camera/Camera.h"

#include <cassert>
#include <sstream>
#include <numbers>
#include <imgui.h>

using namespace MathMatrix;

void Object3d::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// デフォルトカメラをセット
	this->camera = Object3dCommon::GetInstance()->GetDefaultCamera();

	// ワールド変換の初期化
	worldTransform.Initialize();

	InitializeTransformationMatrixData();

	InitializeDirectionalLightData();

	InitializePointLightData();

	InitializeSpotLightData();

	InitializeEnvironmentData();

	InitializeCameraData();
}

void Object3d::Update() {

	// ワールド変換の行列の更新
	worldTransform.UpdateMatrix();

	/// === TransformからWorldMatrixを作る === ///
	Matrix4x4 worldMatrix = worldTransform.GetWorldMatrix();

	// WVP
	Matrix4x4 worldViewProjectionMatrix;

	// カメラがあればviewProjectionをもらってWVPの計算を行う
	if (camera) {

		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();
		worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;

		// カメラのワールド座標を代入
		*cameraData = camera->GetWorldPosition();

		// カメラがなければworldMatrixを代入
	}
	else {

		worldViewProjectionMatrix = worldMatrix;
	}

	transformationMatrixData->WVP = model->GetRootMatrix() * worldViewProjectionMatrix;
	transformationMatrixData->world = model->GetRootMatrix() * worldMatrix;
	transformationMatrixData->worldInverseTranspose = Inverse(model->GetRootMatrix() * worldMatrix);

	directionalLightData->direction = Normalize(directionalLightData->direction);
	spotLightData->direction = Normalize(spotLightData->direction);

	// cosFalloffStartがcosAngleより小さい場合、cosAngleを更新
	if (spotLightData->cosFalloffStart < spotLightData->cosAngle) {
		spotLightData->cosFalloffStart = spotLightData->cosAngle + 0.01f;
	}
}

void Object3d::Draw() {

	if (isDraw) {

		/// === 座標変換行列CBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

		/// === 平行光源CBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(4, directionalLightResource->GetGPUVirtualAddress());

		/// === 点光源CBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointLightResource->GetGPUVirtualAddress());

		/// === スポットライトCBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(6, spotLightResource->GetGPUVirtualAddress());
		
		/// === 環境CBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(7, environmentResource->GetGPUVirtualAddress());

		/// === カメラCBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(8, cameraResource->GetGPUVirtualAddress());

		// 3Dモデルが割り当てられていれば描画する
		if (model) {
			model->Draw();
		}
	}
}

void Object3d::Draw(WorldTransform worldTransform) {

	// 引数のワールド変換をメンバのワールド変換に上書き
	this->worldTransform = worldTransform;

	if (isDraw) {
		
		// ワールド変換の行列の転送
		this->worldTransform.TransferMatrix();

		/// === 平行光源CBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(4, directionalLightResource->GetGPUVirtualAddress());
		/// === 点光源CBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointLightResource->GetGPUVirtualAddress());
		/// === スポットライトCBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(6, spotLightResource->GetGPUVirtualAddress());
		/// === 環境CBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(7, environmentResource->GetGPUVirtualAddress());
		/// === カメラCBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(8, cameraResource->GetGPUVirtualAddress());

		// 3Dモデルが割り当てられていれば描画する
		if (model) {

			model->Draw();
		}
	}
}

void Object3d::ShowImGui() {

	if (ImGui::TreeNode("Object3d")) {

		ImGui::Checkbox("Draw", &isDraw);

		worldTransform.ShowImGui();

		if (ImGui::TreeNode("DirectionalLight")) {
			ImGui::ColorEdit4("Color", &directionalLightData->color.x); // 色
			ImGui::DragFloat3("Direction", &directionalLightData->direction.x, 0.01f); // 向き
			ImGui::DragFloat("Intensity", &directionalLightData->intensity, 0.01f); // 輝度
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("PointLight")) {
			ImGui::ColorEdit4("Color", &pointLightData->color.x); // 色
			ImGui::DragFloat3("Position", &pointLightData->position.x, 0.01f); // 位置
			ImGui::DragFloat("Intensity", &pointLightData->intensity, 0.01f); // 輝度
			ImGui::DragFloat("Distance", &pointLightData->distance, 0.01f); // 最大距離
			ImGui::DragFloat("Decay", &pointLightData->decay, 0.01f); // 減衰率
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("SpotLight")) {
			ImGui::ColorEdit4("Color", &spotLightData->color.x); // 色
			ImGui::DragFloat3("Position", &spotLightData->position.x, 0.01f); // 位置
			ImGui::DragFloat3("Direction", &spotLightData->direction.x, 0.01f); // 向き
			ImGui::DragFloat("Intensity", &spotLightData->intensity, 0.01f); // 輝度
			ImGui::DragFloat("Distance", &spotLightData->distance, 0.01f); // 最大距離
			ImGui::DragFloat("Decay", &spotLightData->decay, 0.01f); // 減衰率
			ImGui::DragFloat("CosAngle", &spotLightData->cosAngle, 0.01f); // 余弦
			ImGui::DragFloat("CosFalloffStart", &spotLightData->cosFalloffStart, 0.01f); // Falloff開始角度
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Environment")) {
			ImGui::SliderFloat("Intensity", &environmentData->intensity, 0.0f, 1.0f); // 環境光の輝度
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void Object3d::InitializeTransformationMatrixData() {

	/// === TransformationMatrixResourceを作る === ///
	transformationMatrixResource = dxUtility->CreateBufferResource(sizeof(TransformationMatrix));

	/// === TransformationMatrixResourceにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる === ///
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	/// === TransformationMatrixDataの初期値を書き込む === ///
	transformationMatrixData->WVP = MakeIdentity4x4(); // 単位行列を書き込む
	transformationMatrixData->world = MakeIdentity4x4(); // 単位行列を書き込む
	transformationMatrixData->worldInverseTranspose = MakeIdentity4x4(); // 単位行列を書き込む
}

void Object3d::InitializeDirectionalLightData() {

	/// === DirectionalLightResourceを作る === ///
	directionalLightResource = dxUtility->CreateBufferResource(sizeof(DirectionalLight));

	/// === DirectionalLightResourceにデータを書き込むためのアドレスを取得してDirectionalLightDataに割り当てる === ///
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	/// === DirectionalLightDataの初期値を書き込む === ///
	directionalLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 白を書き込む
	directionalLightData->direction = Normalize({ 0.0f, -1.0f, 0.0f }); // 向きは下から
	directionalLightData->intensity = 1.0f; // 輝度は最大
}

void Object3d::InitializePointLightData() {

	/// === PointLightResourceを作る === ///
	pointLightResource = dxUtility->CreateBufferResource(sizeof(PointLight));

	/// === PointLightResourceにデータを書き込むためのアドレスを取得してPointLightDataに割り当てる === ///
	pointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData));

	/// === PointLightDataの初期値を書き込む === ///
	pointLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 白を書き込む
	pointLightData->position = { 0.0f, 2.0f, 0.0f }; // 位置は上から
	pointLightData->intensity = 1.0f; // 輝度は最大
	pointLightData->distance = 5.0f; // 最大距離は広く
	pointLightData->decay = 2.0f; // 減衰あり
}

void Object3d::InitializeSpotLightData() {

	/// === SpotLightResourceを作る === ///
	spotLightResource = dxUtility->CreateBufferResource(sizeof(SpotLight));

	/// === SpotLightResourceにデータを書き込むためのアドレスを取得してSpotLightDataに割り当てる === ///
	spotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData));

	/// === SpotLightDataの初期値を書き込む === ///
	spotLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 色は白
	spotLightData->position = { -2.0f, 1.25f, 0.0f }; // 位置は左から
	spotLightData->direction = Normalize({ -1.0f, 0.0f, 0.0f }); // 右向き
	spotLightData->intensity = 4.0f; // 輝度は強め
	spotLightData->distance = 7.0f; // 最大距離は広く
	spotLightData->decay = 2.0f; // 減衰あり
	spotLightData->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f); // π/3
	spotLightData->cosFalloffStart = spotLightData->cosAngle + 0.01f; // cosAngleよりちょっと大きい
}

void Object3d::InitializeEnvironmentData() {

	/// === EnvironmentResourceを作る === ///
	environmentResource = dxUtility->CreateBufferResource(sizeof(Environment));

	/// === EnvironmentResourceにデータを書き込むためのアドレスを取得してEnvironmentDataに割り当てる === ///
	environmentResource->Map(0, nullptr, reinterpret_cast<void**>(&environmentData));

	/// === EnvironmentDataの初期値を書き込む === ///
	environmentData->intensity = 1.0f; // 環境光の輝度は最大
}

void Object3d::InitializeCameraData() {

	/// === CameraResource === ///
	cameraResource = dxUtility->CreateBufferResource(sizeof(Vector3));

	/// === CameraResourceにデータを書き込むためのアドレスを取得してCameraDataに割り当てる === ///
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));

	/// === CameraDataの初期値を書き込む === ///

	// カメラを持っていたら
	if (camera) {
		// カメラのワールド座標を代入
		*cameraData = camera->GetWorldPosition();
	}
}

void Object3d::SetModel(const std::string& filePath) {

	// モデルを検索してセットする
	model = ModelManager::GetInstance()->FindModel(filePath);
}
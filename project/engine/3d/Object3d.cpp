#include "Object3d.h"
#include "Object3dCommon.h"
#include <cassert>
#include <sstream>
#include "math/MathVector.h"
#include "math/MathMatrix.h"
#include "winApp/WinApp.h"
#include "2d/TextureManager.h"
#include "ModelManager.h"
#include "camera/Camera.h"
#include <imgui.h>

using namespace MathMatrix;

void Object3d::Initialize() {

	// デフォルトカメラをセット
	this->camera = Object3dCommon::GetInstance()->GetDefaultCamera();

	InitializeTransformationMatrixData();

	InitializeDirectionalLightData();

	InitializePointLightData();

	InitializeCameraData();

	// Transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Object3d::Update() {

	ImGui::Begin("Object3d");

	if (ImGui::TreeNode("Transform")) {
		ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f); // 大きさ
		ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f); // 回転
		ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f); // 位置
		ImGui::TreePop();
	}

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
		ImGui::DragFloat("Radius", &pointLightData->radius, 0.01f); // 最大距離
		ImGui::DragFloat("Decay", &pointLightData->decay, 0.01f); // 減衰率
		ImGui::TreePop();
	}

	ImGui::End();

	/// === TransformからWorldMatrixを作る === ///
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	// WVP
	Matrix4x4 worldViewProjectionMatrix;

	// カメラがあればviewProjectionをもらってWVPの計算を行う
	if (camera) {

		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();
		worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;

		// カメラのワールド座標を代入
		*cameraData = camera->GetWorldPosition();

	// カメラがなければworldMatrixを代入
	} else {

		worldViewProjectionMatrix = worldMatrix;
	}

	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->world = worldMatrix;

	directionalLightData->direction = Normalize(directionalLightData->direction);
}

void Object3d::Draw() {

	/// === 座標変換行列CBufferの場所を設定 === ///
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	/// === 平行光源CBufferの場所を設定 === ///
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

	/// === 点光源CBufferの場所を設定 === ///
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(4, pointLightResource->GetGPUVirtualAddress());

	/// === カメラCBufferの場所を設定 === ///
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource->GetGPUVirtualAddress());

	// 3Dモデルが割り当てられていれば描画する
	if (model) {
		model->Draw();
	}
}

void Object3d::InitializeTransformationMatrixData() {

	/// === TransformationMatrixResourceを作る === ///
	transformationMatrixResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	/// === TransformationMatrixResourceにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる === ///
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	/// === TransformationMatrixDataの初期値を書き込む === ///
	transformationMatrixData->WVP = MakeIdentity4x4(); // 単位行列を書き込む
	transformationMatrixData->world = MakeIdentity4x4(); // 単位行列を書き込む
}

void Object3d::InitializeDirectionalLightData() {

	/// === DirectionalLightResourceを作る === ///
	directionalLightResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));

	/// === DirectionalLightResourceにデータを書き込むためのアドレスを取得してDirectionalLightDataに割り当てる === ///
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	/// === DirectionalLightDataの初期値を書き込む === ///
	directionalLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 白を書き込む
	directionalLightData->direction = { 0.0f, -1.0f, 0.0f }; // 向きは下から
	directionalLightData->intensity = 1.0f; // 輝度は最大
}

void Object3d::InitializePointLightData() {

	/// === PointLightResourceを作る === ///
	pointLightResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(PointLight));

	/// === PointLightResourceにデータを書き込むためのアドレスを取得してPointLightDataに割り当てる === ///
	pointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData));

	/// === PointLightDataの初期値を書き込む === ///
	pointLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 白を書き込む
	pointLightData->position = { 0.0f, 2.0f, 0.0f }; // 位置は上から
	pointLightData->intensity = 1.0f; // 輝度は最大
	pointLightData->radius = 5.0f; // 最大距離は広く
	pointLightData->decay = 2.0f; // 減衰あり
}

void Object3d::InitializeCameraData() {

	/// === CameraResource === ///
	cameraResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(Vector3));

	/// === CameraResourceにデータを書き込むためのアドレスを取得してCameraDataに割り当てる === ///
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));

	/// === CameraDataの初期値を書き込む === ///
	*cameraData = camera->GetWorldPosition();
}

void Object3d::SetModel(const std::string& filePath) {

	// モデルを検索してセットする
	model = ModelManager::GetInstance()->FindModel(filePath);
}
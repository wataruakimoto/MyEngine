#include "Object3d.h"
#include "Object3dRenderer.h"
#include "MathVector.h"
#include "MathMatrix.h"
#include "WinApp.h"
#include "DirectXUtility.h"
#include "Texture/TextureManager.h"
#include "Model/ModelManager.h"
#include "Camera.h"

#include <cassert>
#include <sstream>
#include <numbers>
#include <imgui.h>

using namespace Engine;
using namespace MathVector;
using namespace MathMatrix;

void Object3d::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// Object3dRendererのインスタンスを取得
	object3dRenderer_ = Object3dRenderer::GetInstance();

	// デフォルトカメラをセット
	this->camera = object3dRenderer_->GetDefaultCamera();

	// ワールド変換の初期化
	worldTransform.Initialize();

	InitializeTransformationMatrixData();

	InitializeCameraData();
}

void Object3d::Update() {

	// ワールド変換の行列の更新
	worldTransform.Update();

	/// === TransformからWorldMatrixを作る === ///
	Matrix4x4 worldMatrix = worldTransform.GetWorldMatrix();

	// WVP
	Matrix4x4 worldViewProjectionMatrix;

	// カメラがあればviewProjectionをもらってWVPの計算を行う
	if (camera) {

		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();
		worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;

		// カメラのワールド座標を代入
		*cameraData = camera->GetWorldTransform().GetWorldPosition();

		// カメラがなければworldMatrixを代入
	}
	else {

		worldViewProjectionMatrix = worldMatrix;
	}

	transformationMatrixData->WVP = model->GetRootMatrix() * worldViewProjectionMatrix;
	transformationMatrixData->world = model->GetRootMatrix() * worldMatrix;
	transformationMatrixData->worldInverseTranspose = Inverse(model->GetRootMatrix() * worldMatrix);
}

void Object3d::Draw() {

	if (isDraw) {

		/// === 座標変換行列CBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(0, transformationMatrixResource->GetGPUVirtualAddress());

		/// === カメラCBufferの場所を設定 === ///
		dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(1, cameraResource->GetGPUVirtualAddress());

		// 3Dモデルが割り当てられていれば描画する
		if (model) {
			model->Draw();
		}
	}
}

void Object3d::ShowImGui() {

#ifdef USE_IMGUI

	if (ImGui::TreeNode("Object3d")) {

		ImGui::Checkbox("Draw", &isDraw);

		worldTransform.ShowImGui();

		ImGui::TreePop();
	}

#endif // USE_IMGUI
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

void Object3d::InitializeCameraData() {

	/// === CameraResource === ///
	cameraResource = dxUtility->CreateBufferResource(sizeof(Vector3));

	/// === CameraResourceにデータを書き込むためのアドレスを取得してCameraDataに割り当てる === ///
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));

	/// === CameraDataの初期値を書き込む === ///

	// カメラを持っていたら
	if (camera) {
		// カメラのワールド座標を代入
		*cameraData = camera->GetWorldTransform().GetWorldPosition();
	}
}
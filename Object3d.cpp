#include "Object3d.h"
#include "Object3dCommon.h"
#include <cassert>
#include <sstream>
#include "MathMatrix.h"
#include "WinApp.h"
#include "TextureManager.h"

using namespace MathMatrix;

void Object3d::Initialize(Object3dCommon* object3dCommon){

	// 引数をメンバ変数に代入
	this->object3dCommon_ = object3dCommon;

	InitializeTransformationMatrixData();

	InitializeDirectionalLightData();

	// Transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,3.14f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform = { {1.0f,1.0f,1.0f},{0.3f,0.0f,0.0f},{0.0f,4.0f,-10.0f} };
}

void Object3d::Update(){

	/// === TransformからWorldMatrixを作る === ///
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	/// === cameraTransformからcameraMatrixを作る === ///
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);

	/// === cameraMatrixからviewMatrixを作る === ///
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);

	/// === ProjectionMatrixを作って透視投影行列を書き込む === ///
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);

	transformationMatrixData->WVP = worldMatrix * viewMatrix * projectionMatrix;
	transformationMatrixData->world = worldMatrix;
}

void Object3d::Draw(){

	/// === 座標変換行列CBufferの場所を設定 === ///
	object3dCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	/// === 平行光源CBufferの場所を設定=== ///
	object3dCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
}

void Object3d::InitializeTransformationMatrixData(){

	/// === TransformationMatrixResourceを作る === ///
	transformationMatrixResource = object3dCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	/// === TransformationMatrixResourceにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる === ///
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	/// === TransformationMatrixDataの初期値を書き込む === ///
	transformationMatrixData->WVP = MakeIdentity4x4(); // 単位行列を書き込む
	transformationMatrixData->world = MakeIdentity4x4(); // 単位行列を書き込む
}

void Object3d::InitializeDirectionalLightData() {

	/// === DirectionalLightResourceを作る === ///
	directionalLightResource = object3dCommon_->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));

	/// === DirectionalLightResourceにデータを書き込むためのアドレスを取得してDirectionalLightDataに割り当てる === ///
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	/// === DirectionalLightDataの初期値を書き込む === ///
	directionalLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 白を書き込む
	directionalLightData->direction = { 0.0f, -1.0f, 0.0f }; // 向きは下から
	directionalLightData->intensity = 1.0f; // 輝度は最大
}
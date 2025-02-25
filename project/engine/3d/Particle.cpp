#include "Particle.h"
#include "Object3dCommon.h"
#include "camera/Camera.h"

void Particle::Initialize() {

	// デフォルトカメラをセット
	this->camera = Object3dCommon::GetInstance()->GetDefaultCamera();

	InitializeTransformationMatrixData();

	InitializeDirectionalLightData();

	InitializePointLightData();

	InitializeSpotLightData();

	InitializeCameraData();

	// Transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Particle::Update() {
}

void Particle::Draw() {

	/// === 座標変換行列CBufferの場所を設定 === ///
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	/// === 平行光源CBufferの場所を設定 === ///
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

	/// === 点光源CBufferの場所を設定 === ///
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(4, pointLightResource->GetGPUVirtualAddress());

	/// === スポットライトCBufferの場所を設定 === ///
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(5, spotLightResource->GetGPUVirtualAddress());

	/// === カメラCBufferの場所を設定 === ///
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(6, cameraResource->GetGPUVirtualAddress());

}

void Particle::InitializeTransformationMatrixData() {
}

void Particle::InitializeDirectionalLightData() {
}

void Particle::InitializePointLightData() {
}

void Particle::InitializeSpotLightData() {
}

void Particle::InitializeCameraData() {
}

#include "Particle.h"
#include "ParticleCommon.h"
#include "2d/TextureManager.h"
#include "ModelManager.h"
#include "camera/Camera.h"
#include "math/MathVector.h"
#include "math/MathMatrix.h"

#include <numbers>
#include <imgui.h>

using namespace MathMatrix;

void Particle::Initialize(const std::string& directorypath, const std::string& filename) {

	// デフォルトカメラをセット
	this->camera = ParticleCommon::GetInstance()->GetDefaultCamera();

	InitializeTransformationMatrixData();

	InitializeDirectionalLightData();

	InitializePointLightData();

	InitializeSpotLightData();

	InitializeCameraData();

	// モデル読み込み
	modelData = ModelManager::LoadObjFile(directorypath, filename);

	// 頂点データ初期化
	InitializeVertexData();

	// マテリアルデータ初期化
	InitializeMaterialData();

	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);

	// 読み込んだテクスチャの番号を取得し、メンバ変数に書き込む
	modelData.material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textureFilePath);

	// Transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Particle::Update() {

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
	}
	else {

		worldViewProjectionMatrix = worldMatrix;
	}

	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->world = worldMatrix;
	transformationMatrixData->worldInverseTranspose = Inverse(worldMatrix);

	directionalLightData->direction = Normalize(directionalLightData->direction);
	spotLightData->direction = Normalize(spotLightData->direction);

	// cosFalloffStartがcosAngleより小さい場合、cosAngleを更新
	if (spotLightData->cosFalloffStart < spotLightData->cosAngle) {
		spotLightData->cosFalloffStart = spotLightData->cosAngle + 0.01f;
	}
}

void Particle::Draw() {

	/// === 座標変換行列CBufferの場所を設定 === ///
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	/// === 平行光源CBufferの場所を設定 === ///
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

	/// === 点光源CBufferの場所を設定 === ///
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(4, pointLightResource->GetGPUVirtualAddress());

	/// === スポットライトCBufferの場所を設定 === ///
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(5, spotLightResource->GetGPUVirtualAddress());

	/// === カメラCBufferの場所を設定 === ///
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(6, cameraResource->GetGPUVirtualAddress());

	// 頂点バッファビューを設定
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// マテリアルCBufferの場所を設定
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// SRVのDescriptorTableの先頭を設定
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVGPUHandle(modelData.material.textureFilePath));

	// 描画(DrawCall)
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}

void Particle::ShowImGui(const char* name) {

	ImGui::Begin(name);

	if (ImGui::TreeNode("Transform")) {
		ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f); // 大きさ
		ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f); // 回転
		ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f); // 位置
		ImGui::TreePop();
	}

	ImGui::End();
}

void Particle::InitializeTransformationMatrixData() {

	/// === TransformationMatrixResourceを作る === ///
	transformationMatrixResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	/// === TransformationMatrixResourceにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる === ///
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	/// === TransformationMatrixDataの初期値を書き込む === ///
	transformationMatrixData->WVP = MakeIdentity4x4(); // 単位行列を書き込む
	transformationMatrixData->world = MakeIdentity4x4(); // 単位行列を書き込む
	transformationMatrixData->worldInverseTranspose = MakeIdentity4x4(); // 単位行列を書き込む
}

void Particle::InitializeDirectionalLightData() {

	/// === DirectionalLightResourceを作る === ///
	directionalLightResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));

	/// === DirectionalLightResourceにデータを書き込むためのアドレスを取得してDirectionalLightDataに割り当てる === ///
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	/// === DirectionalLightDataの初期値を書き込む === ///
	directionalLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 白を書き込む
	directionalLightData->direction = Normalize({ 0.0f, -1.0f, 0.0f }); // 向きは下から
	directionalLightData->intensity = 1.0f; // 輝度は最大
}

void Particle::InitializePointLightData() {

	/// === PointLightResourceを作る === ///
	pointLightResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(PointLight));

	/// === PointLightResourceにデータを書き込むためのアドレスを取得してPointLightDataに割り当てる === ///
	pointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData));

	/// === PointLightDataの初期値を書き込む === ///
	pointLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 白を書き込む
	pointLightData->position = { 0.0f, 2.0f, 0.0f }; // 位置は上から
	pointLightData->intensity = 1.0f; // 輝度は最大
	pointLightData->distance = 5.0f; // 最大距離は広く
	pointLightData->decay = 2.0f; // 減衰あり
}

void Particle::InitializeSpotLightData() {

	/// === SpotLightResourceを作る === ///
	spotLightResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(SpotLight));

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

void Particle::InitializeCameraData() {

	/// === CameraResource === ///
	cameraResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(Vector3));

	/// === CameraResourceにデータを書き込むためのアドレスを取得してCameraDataに割り当てる === ///
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));

	/// === CameraDataの初期値を書き込む === ///
	*cameraData = camera->GetWorldPosition();
}

void Particle::InitializeVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズ 頂点のサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	/// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// 頂点データにリソースをコピー
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void Particle::InitializeMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataの初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->lightingMode = 0; // Lightingをしていない
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
	materialData->shininess = 70.0f;
}

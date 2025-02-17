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

	// モデル読み込み
	modelData = ModelManager::GetInstance()->LoadObjFile(directorypath, filename);

	// デフォルトカメラをセット
	this->camera = ParticleCommon::GetInstance()->GetDefaultCamera();

	InitializeTransformationMatrixData();

	// 頂点データ初期化
	InitializeVertexData();

	// マテリアルデータ初期化
	InitializeMaterialData();

	// Transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);

	// 読み込んだテクスチャの番号を取得し、メンバ変数に書き込む
	modelData.material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textureFilePath);
}

void Particle::Update() {

	/// === TransformからWorldMatrixを作る === ///
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	// WVP
	Matrix4x4 worldViewProjectionMatrix = worldMatrix;

	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->world = worldMatrix;
}

void Particle::Draw() {

	if (isDraw) {

		/// === 座標変換行列CBufferの場所を設定 === ///
		ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

		// 頂点バッファビューを設定
		ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

		// マテリアルCBufferの場所を設定
		ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

		// SRVのDescriptorTableの先頭を設定
		ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVGPUHandle(modelData.material.textureFilePath));

		// 描画(DrawCall)
		ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
	}
}

void Particle::ShowImGui(const char* name) {

	ImGui::Begin(name);

	ImGui::Checkbox("Draw", &isDraw);

	if (ImGui::TreeNode("Transform")) {
		ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f); // 大きさ
		ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f); // 回転
		ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f); // 位置
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Model")) {
		ImGui::ColorEdit4("Color", &materialData->color.x);
		ImGui::Combo("LightingMode", &materialData->lightingMode, "None\0Lambertian Reflection\0Harf Lambert\0Phong Reflection Model\0Blinn-Phong Reflection Model\0PointLight\0SpotLight\0");
		ImGui::DragFloat("Shininess", &materialData->shininess, 0.01f);
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

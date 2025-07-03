#include "Model.h"
#include "Modelcommon.h"
#include "base/DirectXUtility.h"
#include "math/MathMatrix.h"
#include "2d/TextureManager.h"
#include "3d/ModelManager.h"
#include <imgui.h>

using namespace MathMatrix;

void Model::Initialize(const std::string& directorypath, const std::string& filename) {

	// モデル読み込み
	modelData = ModelManager::GetInstance()->FindModelData(directorypath, filename);

	// 頂点データ初期化
	InitializeVertexData();

	// マテリアルデータ初期化
	InitializeMaterialData();

	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData->material.textureFilePath);

	// 読み込んだテクスチャの番号を取得し、メンバ変数に書き込む
	modelData->material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData->material.textureFilePath);
}

void Model::Draw() {

	// 頂点バッファビューを設定
	ModelCommon::GetInstance()->GetdxUtility()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// マテリアルCBufferの場所を設定
	ModelCommon::GetInstance()->GetdxUtility()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// SRVのDescriptorTableの先頭を設定
	ModelCommon::GetInstance()->GetdxUtility()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVGPUHandle(modelData->material.textureFilePath));

	// 描画(DrawCall)
	ModelCommon::GetInstance()->GetdxUtility()->GetCommandList()->DrawInstanced(UINT(modelData->vertices.size()), 1, 0, 0);
}

void Model::ShowImGui() {

#ifdef _DEBUG
	if (ImGui::TreeNode("Model")) {
		ImGui::ColorEdit4("Color", &materialData->color.x);
		ImGui::Combo("LightingMode", &materialData->lightingMode, "None\0Lambertian Reflection\0Harf Lambert\0Phong Reflection Model\0Blinn-Phong Reflection Model\0PointLight\0SpotLight\0");
		ImGui::DragFloat("Shininess", &materialData->shininess, 0.01f);
		ImGui::TreePop();
	}
#endif // _DEBUG
}

void Model::InitializeVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = ModelCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(VertexData) * modelData->vertices.size());

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズ 頂点のサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData->vertices.size());
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	/// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// 頂点データにリソースをコピー
	std::memcpy(vertexData, modelData->vertices.data(), sizeof(VertexData) * modelData->vertices.size());
}

void Model::InitializeMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = ModelCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataの初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->lightingMode = 0; // Lightingをしていない
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
	materialData->shininess = 70.0f;
}
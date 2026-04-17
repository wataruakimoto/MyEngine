#include "Model.h"
#include "DirectXUtility.h"
#include "MathMatrix.h"
#include "Texture/TextureManager.h"
#include "Model/ModelManager.h"
#include <imgui.h>

using namespace Engine;
using namespace MathMatrix;

void Model::Initialize(const std::string& directoryName, const std::string& fileName) {

	// DXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// モデルデータを検索
	modelData = ModelManager::GetInstance()->FindModelData(directoryName, fileName);

	// 頂点データ初期化
	InitializeVertexData();

	// マテリアルデータ初期化
	InitializeMaterialData();

	// 読み込んだテクスチャの番号を取得し、メンバ変数に書き込む
	modelData->material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData->material.textureFilePath);
}

void Model::Draw() {

	// 頂点バッファビューを設定
	dxUtility->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// マテリアルCBufferの場所を設定
	dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(2, materialResource->GetGPUVirtualAddress());

	// SRVのDescriptorTableを設定
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(7, TextureManager::GetInstance()->GetSRVGPUHandle(modelData->material.textureFilePath));

	// SRVのDescriptorTableを設定
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(8, TextureManager::GetInstance()->GetSRVGPUHandle(environmentMapFilePath));

	// 描画(DrawCall)
	dxUtility->GetCommandList()->DrawInstanced(UINT(modelData->vertices.size()), 1, 0, 0);
}

void Model::ShowImGui() {

#ifdef USE_IMGUI

	if (ImGui::TreeNode("モデル")) {
		ImGui::Text("ファイルパス: %s", modelData->material.textureFilePath.c_str());
		ImGui::ColorEdit4("色", &materialData->color.x);
		ImGui::ColorEdit3("鏡面反射色", &materialData->specularColor.x);
		ImGui::Combo("拡散反射の設定", (int*)&materialData->diffuseSetting, "なし\0Lambert反射\0Half-Lambert反射\0");
		ImGui::Combo("鏡面反射の設定", (int*)&materialData->specularSetting, "ない\0Phong反射\0Blinn-Phong反射\0");
		bool useEnv = materialData->useEnvironmentMap;
		if (ImGui::Checkbox("環境マップの設定", &useEnv)) {
			materialData->useEnvironmentMap = useEnv ? 1 : 0;
		}
		ImGui::DragFloat("明るさ", &materialData->shininess, 0.01f);
		ImGui::TreePop();
	}

#endif // USE_IMGUI
}

void Model::InitializeVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = dxUtility->CreateBufferResource(sizeof(VertexData) * modelData->vertices.size());

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
	materialResource = dxUtility->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataの初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 白
	materialData->specularColor = Vector3(1.0f, 1.0f, 1.0f); // 白
	materialData->diffuseSetting = 0; // 拡散反射は使わない
	materialData->specularSetting = 0; // 鏡面反射は使わない
	materialData->useEnvironmentMap = 0; // 環境マップは使わない
	materialData->shininess = 70.0f;
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}
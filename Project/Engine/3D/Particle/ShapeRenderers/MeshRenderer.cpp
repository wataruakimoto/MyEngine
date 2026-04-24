#include "MeshRenderer.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "Texture/TextureManager.h"
#include "Model/ModelManager.h"
#include "MathMatrix.h"

using namespace Engine;
using namespace MathMatrix;

void MeshRenderer::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// SrvManagerのインスタンスを取得
	srvManager = SrvManager::GetInstance();

	// TextureManagerのインスタンスを取得
	textureManager = TextureManager::GetInstance();

	// ModelManagerのインスタンスを取得
	modelManager = ModelManager::GetInstance();

	// モデルデータをマネージャから検索して取得
	modelData = modelManager->FindModelData("Player", "player.obj");

	// 頂点データ生成
	GenerateVertexData();

	// マテリアルデータ生成
	GenerateMaterialData();
}

void MeshRenderer::Update() {
}

void MeshRenderer::Draw(uint16_t instanceCount, uint16_t instanceSrvIndex, const std::string& texturePath) {

	// 頂点バッファビューを設定
	dxUtility->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	/// === InstanceDataのCBufferの場所を設定 === ///
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvManager->GetGPUDescriptorHandle(instanceSrvIndex));

	// マテリアルCBufferの場所を設定
	dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(1, materialResource->GetGPUVirtualAddress());

	// SRVのDescriptorTableの先頭を設定
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetSRVGPUHandle(modelData->material.textureFilePath));

	// 描画(DrawCall)
	dxUtility->GetCommandList()->DrawInstanced(UINT(modelData->vertices.size()), instanceCount, 0, 0);
}

void MeshRenderer::GenerateVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = dxUtility->CreateBufferResource(sizeof(VertexData) * modelData->vertices.size());

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズ 頂点のサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData->vertices.size());

	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	/// === VertexResourceに初期値を書き込む === ///

	// 頂点データをモデルデータの頂点データで上書き
	std::memcpy(vertexData, modelData->vertices.data(), sizeof(VertexData) * modelData->vertices.size());
}

void MeshRenderer::GenerateIndexData() {

	// 参照は使わないため何もしない
}

void MeshRenderer::GenerateMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = dxUtility->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataに初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 白
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}

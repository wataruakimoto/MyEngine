#include "PlaneRenderer.h"
#include "DirectXUtility.h"
#include "Texture/TextureManager.h"
#include "SrvManager.h"
#include "MathMatrix.h"
#include "Camera.h"

#include <numbers>

using namespace MathMatrix;

void PlaneRenderer::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// TextureManagerのインスタンスを取得
	textureManager = TextureManager::GetInstance();

	// SrvManagerのインスタンスを取得
	srvManager = SrvManager::GetInstance();

	// 頂点データ生成
	GenerateVertexData();

	// 参照データ生成
	GenerateIndexData();

	// マテリアルデータ生成
	GenerateMaterialData();
}

void PlaneRenderer::Draw(uint16_t instanceCount, uint16_t instanceSrvIndex, const std::string& texturePath) {

	// 頂点バッファビューを設定
	dxUtility->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 参照バッファビューを設定
	dxUtility->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// マテリアルCBufferの場所を設定
	dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	/// === InstanceDataのCBufferの場所を設定 === ///
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager->GetGPUDescriptorHandle(instanceSrvIndex));

	// SRVのDescriptorTableの先頭を設定
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetSRVGPUHandle(texturePath));

	// 描画(DrawCall)
	dxUtility->GetCommandList()->DrawIndexedInstanced(36, instanceCount, 0, 0, 0);
}

void PlaneRenderer::GenerateVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = dxUtility->CreateBufferResource(sizeof(VertexData) * 6);

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズ 頂点のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;

	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	/// === VertexResourceに初期値を書き込む(4頂点) === ///

	// 左下
	vertexData[0].position = { -0.5f, 0.5f, 0.0f, 1.0f };
	vertexData[0].texcoord = { 0.0f, 1.0f };
	// 左上
	vertexData[1].position = { -0.5f, -0.5f, 0.0f, 1.0f };
	vertexData[1].texcoord = { 0.0f, 0.0f };
	// 右下
	vertexData[2].position = { 0.5f, 0.5f, 0.0f, 1.0f };
	vertexData[2].texcoord = { 1.0f, 1.0f };
	// 右上
	vertexData[3].position = { 0.5f, -0.5f, 0.0f, 1.0f };
	vertexData[3].texcoord = { 1.0f, 0.0f };
}

void PlaneRenderer::GenerateIndexData() {

	/// === IndexResourceを作る === ///
	indexResource = dxUtility->CreateBufferResource(sizeof(uint32_t) * 6);

	/// === IBVを作成する(値を設定するだけ) === ///

	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;

	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	/// === IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる === ///
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	/// === IndexResourceに初期値を書き込む(6個分)=== ///

	indexData[0] = 0; // 左下
	indexData[1] = 1; // 左上
	indexData[2] = 2; // 右下
	indexData[3] = 1; // 左上
	indexData[4] = 3; // 右上
	indexData[5] = 2; // 右下
}

void PlaneRenderer::GenerateMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = dxUtility->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataに初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}

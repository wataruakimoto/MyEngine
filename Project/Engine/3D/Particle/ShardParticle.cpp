#include "ShardParticle.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "Texture/TextureManager.h"
#include "ParticleCommon.h"
#include "MathMatrix.h"

using namespace MathMatrix;

void ShardParticle::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// 頂点データ生成
	GenerateVertexData();

	// 参照データ生成
	GenerateIndexData();

	// マテリアルデータ生成
	GenerateMaterialData();
}

void ShardParticle::Update() {
}

void ShardParticle::Draw(ParticleGroup* group) {

	// 頂点バッファビューを設定
	dxUtility->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 参照バッファビューを設定
	dxUtility->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// マテリアルCBufferの場所を設定
	dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// SRVのDescriptorTableの先頭を設定
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVGPUHandle(group->textureFilePath));

	/// === パーティクルCBufferの場所を設定 === ///
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(1, SrvManager::GetInstance()->GetGPUDescriptorHandle(group->srvIndex));

	// 描画(DrawCall)
	dxUtility->GetCommandList()->DrawIndexedInstanced(12, group->numInstance, 0, 0, 0);
}

void ShardParticle::GenerateVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = dxUtility->CreateBufferResource(sizeof(VertexData) * 4);

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズ 頂点のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;

	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	/// === VertexResourceに初期値を書き込む(4頂点) === ///

	// 先端
	vertexData[0].position = { 0.0f, 1.0f, 0.0f, 1.0f };
	vertexData[0].texcoord = { 0.5f, 0.0f };
	vertexData[0].normal = { 0.0f, 0.0f, 0.0f };

	// 底面左前
	vertexData[1].position = { -0.5f, 0.0f, 0.5f, 1.0f };
	vertexData[1].texcoord = { 0.0f, 1.0f };
	vertexData[1].normal = { 0.0f, 0.0f, 0.0f };

	// 底面右前
	vertexData[2].position = { 0.5f, 0.0f, 0.5f, 1.0f };
	vertexData[2].texcoord = { 1.0f, 1.0f };
	vertexData[2].normal = { 0.0f, 0.0f, 0.0f };

	// 底面奥
	vertexData[3].position = { 0.0f, 0.0f, -0.5f, 1.0f };
	vertexData[3].texcoord = { 0.5f, 0.8f };
	vertexData[3].normal = { 0.0f, 0.0f, 0.0f };
}

void ShardParticle::GenerateIndexData() {

	/// === IndexResourceを作る === ///
	indexResource = dxUtility->CreateBufferResource(sizeof(uint32_t) * 12);

	/// === IBVを作成する(値を設定するだけ) === ///

	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 12;

	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	/// === IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる === ///
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	/// === IndexResourceに初期値を書き込む(4個分)=== ///

	// 前面三角形
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;

	// 右側面三角形
	indexData[3] = 0;
	indexData[4] = 2;
	indexData[5] = 3;

	// 左側面三角形
	indexData[6] = 0;
	indexData[7] = 3;
	indexData[8] = 1;

	// 底面三角形
	indexData[9] = 1;
	indexData[10] = 3;
	indexData[11] = 2;
}

void ShardParticle::GenerateMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = dxUtility->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataに初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}

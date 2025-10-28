#include "CubeParticle.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "Texture/TextureManager.h"
#include "ParticleCommon.h"
#include "MathMatrix.h"

using namespace MathMatrix;

void CubeParticle::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// 頂点データ生成
	GenerateVertexData();

	// 参照データ生成
	GenerateIndexData();

	// マテリアルデータ生成
	GenerateMaterialData();
}

void CubeParticle::Update() {
}

void CubeParticle::Draw(ParticleGroup* group) {

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
	dxUtility->GetCommandList()->DrawIndexedInstanced(36, group->numInstance, 0, 0, 0);
}

void CubeParticle::GenerateVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = dxUtility->CreateBufferResource(sizeof(VertexData) * 8);

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズ 頂点のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 8;

	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	/// === VertexResourceに初期値を書き込む(8頂点) === ///

	// 左下奥
	vertexData[0].position = { -0.5f, -0.5f, -0.5f, 1.0f };
	vertexData[0].texcoord = { 0.0f, 1.0f };
	vertexData[0].normal = { 0.0f, 0.0f, 0.0f };

	// 右下奥
	vertexData[1].position = { 0.5f, -0.5f, -0.5f, 1.0f };
	vertexData[1].texcoord = { 1.0f, 1.0f };
	vertexData[1].normal = { 0.0f, 0.0f, 0.0f };

	// 左上奥
	vertexData[2].position = { -0.5f, 0.5f, -0.5f, 1.0f };
	vertexData[2].texcoord = { 0.0f, 0.0f };
	vertexData[2].normal = { 0.0f, 0.0f, 0.0f };

	// 右上奥
	vertexData[3].position = { 0.5f, 0.5f, -0.5f, 1.0f };
	vertexData[3].texcoord = { 1.0f, 0.0f };
	vertexData[3].normal = { 0.0f, 0.0f, 0.0f };

	// 左下手前
	vertexData[4].position = { -0.5f, -0.5f, 0.5f, 1.0f };
	vertexData[4].texcoord = { 0.0f, 1.0f };
	vertexData[4].normal = { 0.0f, 0.0f, 0.0f };

	// 右下手前
	vertexData[5].position = { 0.5f, -0.5f, 0.5f, 1.0f };
	vertexData[5].texcoord = { 1.0f, 1.0f };
	vertexData[5].normal = { 0.0f, 0.0f, 0.0f };

	// 左上手前
	vertexData[6].position = { -0.5f, 0.5f, 0.5f, 1.0f };
	vertexData[6].texcoord = { 0.0f, 0.0f };
	vertexData[6].normal = { 0.0f, 0.0f, 0.0f };

	// 右上手前
	vertexData[7].position = { 0.5f, 0.5f, 0.5f, 1.0f };
	vertexData[7].texcoord = { 1.0f, 0.0f };
	vertexData[7].normal = { 0.0f, 0.0f, 0.0f };
}

void CubeParticle::GenerateIndexData() {

	/// === IndexResourceを作る === ///
	indexResource = dxUtility->CreateBufferResource(sizeof(uint32_t) * 36);

	/// === IBVを作成する(値を設定するだけ) === ///

	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 36;

	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	/// === IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる === ///
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	/// === IndexResourceに初期値を書き込む(6個分)=== ///

	// 前面 (+Z方向)
	indexData[0] = 4; indexData[1] = 6; indexData[2] = 5;

	indexData[3] = 5; indexData[4] = 6; indexData[5] = 7;

	// 背面 (-Z方向)
	indexData[6] = 1; indexData[7] = 3; indexData[8] = 0;

	indexData[9] = 0; indexData[10] = 3; indexData[11] = 2;

	// 右面 (+X方向)
	indexData[12] = 5; indexData[13] = 7; indexData[14] = 1;

	indexData[15] = 1; indexData[16] = 7; indexData[17] = 3;

	// 左面 (-X方向)
	indexData[18] = 0; indexData[19] = 2; indexData[20] = 4;

	indexData[21] = 4; indexData[22] = 2; indexData[23] = 6;

	// 上面 (+Y方向)
	indexData[24] = 2; indexData[25] = 3; indexData[26] = 6;

	indexData[27] = 6; indexData[28] = 3; indexData[29] = 7;

	// 下面 (-Y方向)
	indexData[30] = 4; indexData[31] = 5; indexData[32] = 0;

	indexData[33] = 0; indexData[34] = 5; indexData[35] = 1;
}

void CubeParticle::GenerateMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = dxUtility->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataに初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}

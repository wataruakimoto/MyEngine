#include "DefaultParticle.h"
#include "Base/DirectXUtility.h"
#include "ParticleCommon.h"
#include "Math/MathMatrix.h"

using namespace MathMatrix;

void DefaultParticle::GenerateVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(VertexData) * 6);


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

void DefaultParticle::GenerateIndexData() {

	/// === IndexResourceを作る === ///
	indexResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(uint32_t) * 6);

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

void DefaultParticle::GenerateMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataに初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}

void DefaultParticle::Draw() {

	// 頂点バッファビューを設定
	ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 参照バッファビューを設定
	ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// マテリアルCBufferの場所を設定
	ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// 各パーティクルグループの描画
	for (auto& [key, particleGroup] : particleGroups) {

		// SRVのDescriptorTableの先頭を設定
		ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVGPUHandle(particleGroup.textureFilePath));

		/// === パーティクルCBufferの場所を設定 === ///
		ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->SetGraphicsRootDescriptorTable(1, SrvManager::GetInstance()->GetGPUDescriptorHandle(particleGroup.srvIndex));

		// 描画(DrawCall)
		ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->DrawIndexedInstanced(6, particleGroup.numInstance, 0, 0, 0);
		
	}
}

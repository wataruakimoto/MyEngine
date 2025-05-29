#include "RingParticle.h"
#include "Base/DirectXUtility.h"
#include "Base/SrvManager.h"
#include "2D/TextureManager.h"
#include "ParticleCommon.h"
#include "Math/MathMatrix.h"
#include <imgui.h>

using namespace MathMatrix;

void RingParticle::Initialize() {

	// 頂点データ生成
	GenerateVertexData();

	// 参照データ生成
	GenerateIndexData();

	// マテリアルデータ生成
	GenerateMaterialData();
}

void RingParticle::Update() {

	for (uint32_t index = 0; index < kRingDivide; ++index) {

		uint32_t vertexIndex = index * 4; // 頂点のインデックス

		vertexData[0 + vertexIndex].texcoord.x += kDeltaTime * kUVSpeed;
		vertexData[1 + vertexIndex].texcoord.x += kDeltaTime * kUVSpeed;
		vertexData[2 + vertexIndex].texcoord.x += kDeltaTime * kUVSpeed;
		vertexData[3 + vertexIndex].texcoord.x += kDeltaTime * kUVSpeed;

		// すべてのtexcoordの値を0.0fから1.0fに収める
		if (vertexData[0 + vertexIndex].texcoord.x > 1.0f) {

			vertexData[0 + vertexIndex].texcoord.x -= 1.0f;
			vertexData[1 + vertexIndex].texcoord.x -= 1.0f;
			vertexData[2 + vertexIndex].texcoord.x -= 1.0f;
			vertexData[3 + vertexIndex].texcoord.x -= 1.0f;
		}
	}
}

void RingParticle::Draw(ParticleGroup* group) {

	// 頂点バッファビューを設定
	ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 参照バッファビューを設定
	ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// マテリアルCBufferの場所を設定
	ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// SRVのDescriptorTableの先頭を設定
	ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVGPUHandle(group->textureFilePath));

	/// === パーティクルCBufferの場所を設定 === ///
	ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->SetGraphicsRootDescriptorTable(1, SrvManager::GetInstance()->GetGPUDescriptorHandle(group->srvIndex));

	// 描画(DrawCall)
	ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->DrawIndexedInstanced(6 * kRingDivide, group->numInstance, 0, 0, 0);
}

void RingParticle::GenerateVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(VertexData) * 4 * kRingDivide);

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズ 頂点のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4 * kRingDivide;

	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	for (uint32_t index = 0; index < kRingDivide; ++index) {

		float sin = std::sin(index * radianPerDivide);
		float cos = std::cos(index * radianPerDivide);
		float sinNext = std::sin((index + 1) * radianPerDivide);
		float cosNext = std::cos((index + 1) * radianPerDivide);
		float uv = float(index) / float(kRingDivide);
		float uvNext = float(index + 1) / float(kRingDivide);

		uint32_t vertexIndex = index * 4; // 頂点のインデックス

		/// === VertexResourceに初期値を書き込む(4頂点) === ///

		// 左下
		vertexData[0 + vertexIndex].position = { -sin * kInnerRadius, cos * kInnerRadius, 0.0f, 1.0f };
		vertexData[0 + vertexIndex].texcoord = { uv, 1.0f };
		// 左上
		vertexData[1 + vertexIndex].position = { -sin * kOuterRadius, cos * kOuterRadius, 0.0f, 1.0f };
		vertexData[1 + vertexIndex].texcoord = { uv, 0.0f };
		// 右下
		vertexData[2 + vertexIndex].position = { -sinNext * kInnerRadius, cosNext * kInnerRadius, 0.0f, 1.0f };
		vertexData[2 + vertexIndex].texcoord = { uvNext, 1.0f };
		// 右上
		vertexData[3 + vertexIndex].position = { -sinNext * kOuterRadius, cosNext * kOuterRadius, 0.0f, 1.0f };
		vertexData[3 + vertexIndex].texcoord = { uvNext, 0.0f };
	}
}

void RingParticle::GenerateIndexData() {

	/// === IndexResourceを作る === ///
	indexResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(uint32_t) * 6 * kRingDivide);

	/// === IBVを作成する(値を設定するだけ) === ///

	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6 * kRingDivide;

	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	/// === IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる === ///
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	/// === IndexResourceに初期値を書き込む(6個分)=== ///

	for (uint32_t index = 0; index < kRingDivide; ++index) {

		indexData[index * 6 + 0] = index * 4 + 0; // 左下
		indexData[index * 6 + 1] = index * 4 + 1; // 左上
		indexData[index * 6 + 2] = index * 4 + 2; // 右下
		indexData[index * 6 + 3] = index * 4 + 1; // 左上
		indexData[index * 6 + 4] = index * 4 + 3; // 右上
		indexData[index * 6 + 5] = index * 4 + 2; // 右下
	}
}

void RingParticle::GenerateMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataに初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}

#include "RingRenderer.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "Texture/TextureManager.h"
#include "MathMatrix.h"

using namespace Engine;
using namespace MathMatrix;

void RingRenderer::Initialize() {

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

void RingRenderer::Update() {

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

void RingRenderer::Draw(uint16_t instanceCount, uint16_t instanceSrvIndex, const std::string& texturePath) {

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
	dxUtility->GetCommandList()->DrawIndexedInstanced(6 * kRingDivide, instanceCount, 0, 0, 0);
}

void RingRenderer::GenerateVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = dxUtility->CreateBufferResource(sizeof(VertexData) * 4 * kRingDivide);

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

void RingRenderer::GenerateIndexData() {

	/// === IndexResourceを作る === ///
	indexResource = dxUtility->CreateBufferResource(sizeof(uint32_t) * 6 * kRingDivide);

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

void RingRenderer::GenerateMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = dxUtility->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataに初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}

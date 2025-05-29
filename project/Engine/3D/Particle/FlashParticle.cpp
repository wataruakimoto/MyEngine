#include "FlashParticle.h"
#include "Base/DirectXUtility.h"
#include "Base/SrvManager.h"
#include "2D/TextureManager.h"
#include "ParticleCommon.h"
#include "Math/MathMatrix.h"

using namespace MathMatrix;

void FlashParticle::Initialize() {

	// 頂点データ生成
	GenerateVertexData();

	// 参照データ生成
	GenerateIndexData();

	// マテリアルデータ生成
	GenerateMaterialData();
}

void FlashParticle::Update() {
}

void FlashParticle::Draw(ParticleGroup* group) {

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
	ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->DrawIndexedInstanced(3 * kNumSpike, group->numInstance, 0, 0, 0);
}

void FlashParticle::GenerateVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(VertexData) * 3 * kNumSpike);

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズ 頂点のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 3 * kNumSpike;

	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// スパイクを作成
	GenerateSpike();
}

void FlashParticle::GenerateIndexData() {

	/// === IndexResourceを作る === ///
	indexResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(uint32_t) * 3 * kNumSpike);

	/// === IBVを作成する(値を設定するだけ) === ///

	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズはインデックス3つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 3 * kNumSpike;

	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	/// === IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる === ///
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	/// === IndexResourceに初期値を書き込む(3個分)=== ///

	for (uint32_t index = 0; index < kNumSpike; ++index) {

		indexData[index * 3 + 0] = index * 3 + 0; // 上
		indexData[index * 3 + 1] = index * 3 + 1; // 左
		indexData[index * 3 + 2] = index * 3 + 2; // 右
	}
}

void FlashParticle::GenerateMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataに初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}

void FlashParticle::GenerateSpike() {

	// 乱数生成
	randomEngine.seed(seedGenerator());

	// ランダムな値を生成するためのエンジンを初期化
	std::uniform_real_distribution<float> dist(kMinLength, kMaxLength);

	// 分割数と同じ数行う
	for (uint32_t index = 0; index < kNumSpike; ++index) {
		
		float angle = index * angleStep;
		float angleNext = angle + angleStep * 0.5f;
		float sin = std::sin(angle);
		float cos = std::cos(angle);
		float sinNext = std::sin(angleNext);
		float cosNext = std::cos(angleNext);

		float LengthL = dist(randomEngine);
		float LengthR = dist(randomEngine);

		uint32_t vertexIndex = index * 3; // 頂点のインデックス

		/// === VertexResourceに初期値を書き込む(4頂点) === ///

		// 上
		vertexData[0 + vertexIndex].position = { 0.0f, 0.0f, 0.0f, 1.0f };
		vertexData[0 + vertexIndex].texcoord = { 0.5f, 0.0f };
		// 左
		vertexData[1 + vertexIndex].position = { sin * LengthL, cos * LengthL, 0.0f, 1.0f };
		vertexData[1 + vertexIndex].texcoord = { 0.0f, 1.0f };
		// 右
		vertexData[2 + vertexIndex].position = { sinNext * LengthR, cosNext * LengthR, 0.0f, 1.0f };
		vertexData[2 + vertexIndex].texcoord = { 1.0f, 1.0f };
	}
}

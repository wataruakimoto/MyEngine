#include "FlashParticle.h"
#include "Base/DirectXUtility.h"
#include "Base/SrvManager.h"
#include "2D/TextureManager.h"
#include "ParticleCommon.h"
#include "Math/MathMatrix.h"

using namespace MathMatrix;

void FlashParticle::Initialize() {

	// 乱数生成
	randomEngine.seed(seedGenerator());

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
	ParticleCommon::GetInstance()->GetdxUtility()->GetCommandList()->DrawIndexedInstanced(6 * kNumSpike, group->numInstance, 0, 0, 0);
}

void FlashParticle::GenerateVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(VertexData) * 4 * kNumSpike);

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズ 頂点のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4 * kNumSpike;

	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// スパイクを作成
	GenerateSpike();
}

void FlashParticle::GenerateIndexData() {

	/// === IndexResourceを作る === ///
	indexResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(uint32_t) * 6 * kNumSpike);

	/// === IBVを作成する(値を設定するだけ) === ///

	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズはインデックス3つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6 * kNumSpike;

	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	/// === IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる === ///
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	/// === IndexResourceに初期値を書き込む(4個分)=== ///

	for (uint32_t index = 0; index < kNumSpike; ++index) {

		// 右の三角形
		indexData[index * 6 + 0] = index * 4 + 0; // 上
		indexData[index * 6 + 1] = index * 4 + 1; // 右
		indexData[index * 6 + 2] = index * 4 + 2; // 下
		// 左の三角形
		indexData[index * 6 + 3] = index * 4 + 2; // 下
		indexData[index * 6 + 4] = index * 4 + 3; // 左
		indexData[index * 6 + 5] = index * 4 + 0; // 上
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

	// 分割数と同じ数行う
	for (uint32_t index = 0; index < kNumSpike; ++index) {

		float angle = index * angleStep;
		float nextAngle = (index + 1) * angleStep;

		// 上方向
		std::uniform_real_distribution<float> topLengthDist(kMinLength, kMaxLength);
		float topLength = topLengthDist(randomEngine);

		Vector2 top = { std::cos((angle + nextAngle) * 0.5f) * topLength, std::sin((angle + nextAngle) * 0.5f) * topLength };

		// 下方向
		Vector2 center = { 0.0f, 0.0f };

		// 右方向
		Vector2 right = {};

		// インデックスが0(１つ目のスパイク)の時
		if (index == 0) {

			std::uniform_real_distribution<float> rightLengthDist(kMinLength, kMaxLength);
			float rightLength = rightLengthDist(randomEngine);

			// 前の右側の座標がないので、ランダムに生成
			right = { std::cos(angle) * rightLength, std::sin(angle) * rightLength };

			// 最初の右側の座標を保存しておく
			firstRight = right;
		}
		// それ以外(２個目以降のスパイク)の時
		else {

			// 前の左側側の座標を使う
			right = preLeft;
		}

		// 左方向
		Vector2 left = {};

		// インデックスがスパイクの数-1(最後のスパイク)の時
		if (index == kNumSpike - 1) {

			// 最初の右側の座標を使う
			left = firstRight;
		}
		// それ以外(最後のスパイク以外)の時
		else {

			std::uniform_real_distribution<float> leftLengthDist(kMinLength, kMaxLength);
			float leftLength = leftLengthDist(randomEngine);

			left = { std::cos(nextAngle) * leftLength, std::sin(nextAngle) * leftLength };

			preLeft = left; // 今回の結果を保存しておく
		}

		/// === VertexResourceに初期値を書き込む(4頂点) === ///

		uint32_t vertexIndex = index * 4; // 頂点のインデックス

		// 上
		vertexData[vertexIndex + 0].position = { top.x, top.y, 0.0f, 1.0f };
		vertexData[vertexIndex + 0].texcoord = { 0.0f, 0.0f };
		// 右
		vertexData[vertexIndex + 1].position = { right.x, right.y, 0.0f, 1.0f };
		vertexData[vertexIndex + 1].texcoord = { 1.0f, 0.0f };
		// 下
		vertexData[vertexIndex + 2].position = { center.x, center.y, 0.0f, 1.0f };
		vertexData[vertexIndex + 2].texcoord = { 1.0f, 1.0f };
		// 左
		vertexData[vertexIndex + 3].position = { left.x, left.y, 0.0f, 1.0f };
		vertexData[vertexIndex + 3].texcoord = { 0.0f, 1.0f };
	}
}

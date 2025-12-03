#include "CubeRenderer.h"
#include "DirectXUtility.h"
#include "Texture/TextureManager.h"
#include "SrvManager.h"
#include "MathMatrix.h"
#include "Camera.h"

#include <numbers>

using namespace MathMatrix;

void CubeRenderer::Initialize() {

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

	// インスタンスデータ生成
	GenerateInstanceData();
}

void CubeRenderer::BeginFrame() {

	// カウンターをリセット
	currentInstanceIndex = 0;
}

void CubeRenderer::Draw(const std::list<ParticleInstance>& particles, const Camera& camera) {

	// リストが空なら何もしない
	if (particles.empty()) return;

	// 今回描画する数
	uint32_t count = static_cast<uint32_t>(particles.size());

	// 今回書き込むと最大数を超えてしまうなら
	if (count + currentInstanceIndex >= kMaxInstanceCount) {

		// 超えてしまったときの処理を行う

		// 中止
		assert(false);
	}

	//  インスタンスの数をカウント
	uint32_t index = 0;

	// カメラからViewProjectionを受け取る
	Matrix4x4 viewProjectionMatrix = camera.GetViewProjectionMatrix();

	// 180度回転行列を作成
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

	// ビルボード行列を計算
	Matrix4x4 billboardMatrix = backToFrontMatrix * camera.GetWorldMatrix();

	// 行列の平行移動成分を排除する
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	for (const auto& particle : particles) {

		// 現在のオフセットから今回の分加算する
		uint32_t targetIndex = currentInstanceIndex + index;

		// ワールド行列を初期化
		Matrix4x4 worldMatrix = MakeIdentity4x4();

		// ビルボードするなら
		if (particle.setting->useBillboard) {

			// Scale行列
			Matrix4x4 scaleMatrix = MakeScaleMatrix(particle.scale);

			// Z軸回転行列
			Matrix4x4 rotateZMatrix = MakeRotateZMatrix(particle.rotate.z);

			// Translate行列
			Matrix4x4 translateMatrix = MakeTranslateMatrix(particle.translate);

			// ワールド行列計算
			worldMatrix = scaleMatrix * rotateZMatrix * billboardMatrix * translateMatrix;
		}
		else {

			// ワールド行列計算
			worldMatrix = MakeAffineMatrix(particle.scale, particle.rotate, particle.translate);
		}

		// ワールドビュー射影行列計算
		Matrix4x4 wvpMatrix = Multiply(worldMatrix, viewProjectionMatrix);

		// ずらしたあとの先頭から書き込む
		instanceData[targetIndex].world = worldMatrix;
		instanceData[targetIndex].WVP = wvpMatrix;
		instanceData[targetIndex].color = particle.color;

		// インスタンス数をインクリメント
		index++;
	}

	// 頂点バッファビューを設定
	dxUtility->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 参照バッファビューを設定
	dxUtility->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// マテリアルCBufferの場所を設定
	dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	/// === InstanceDataのCBufferの場所を設定 === ///
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager->GetGPUDescriptorHandle(srvIndex));

	// パーティクルのテクスチャパスを取得 (同じテクスチャのリストなので先頭だけでOK)
	std::string textureFullPath = particles.front().setting->textureFullPath;

	// SRVのDescriptorTableの先頭を設定
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetSRVGPUHandle(textureFullPath));

	// 描画(DrawCall)
	dxUtility->GetCommandList()->DrawIndexedInstanced(36, count, 0, 0, currentInstanceIndex);

	// オフセット分を加算
	currentInstanceIndex += count;
}

void CubeRenderer::GenerateVertexData() {

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

void CubeRenderer::GenerateIndexData() {

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

void CubeRenderer::GenerateMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = dxUtility->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataに初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}

void CubeRenderer::GenerateInstanceData() {

	/// === InstanceResourceを作る === ///
	instanceResource = dxUtility->CreateBufferResource(sizeof(InstanceData) * kMaxInstanceCount);

	/// === InstanceResourceにデータを書き込むためのアドレスを取得してInstanceDataに割り当てる === ///
	instanceResource->Map(0, nullptr, reinterpret_cast<void**>(&instanceData));

	/// === InstanceDataに初期値を書き込む === ///
	for (uint32_t index = 0; index < kMaxInstanceCount; ++index) {

		instanceData[index].WVP = MakeIdentity4x4(); // 単位行列を書き込む
		instanceData[index].world = MakeIdentity4x4(); // 単位行列を書き込む
		instanceData[index].color = { 1.0f,1.0f,1.0f,1.0f }; // 白を書き込む
	}

	// SRVインデックスを取得
	srvIndex = srvManager->Allocate();

	/// === SRVを作成 === ///
	srvManager->CreateSRVforStructuredBuffer(srvIndex, instanceResource.Get(), kMaxInstanceCount, sizeof(InstanceData));
}

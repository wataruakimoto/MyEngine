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

	// インスタンスデータ生成
	GenerateInstanceData();
}

void PlaneRenderer::Draw(const std::list<ParticleInstance>& particles, const Camera& camera) {

	// リストが空なら何もしない
	if (particles.empty()) return;

	//  インスタンスの数をカウント
	uint32_t instanceCount = 0;

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

	for(const auto& particle : particles) {

		// 最大数を超えたら抜ける
		if (instanceCount >= kMaxInstanceCount) break;

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

		// インスタンスデータに転送
		instanceData[instanceCount].world = worldMatrix;
		instanceData[instanceCount].WVP = wvpMatrix;
		instanceData[instanceCount].color = particle.color;

		// インスタンス数をインクリメント
		++instanceCount;
	}

	// 頂点バッファビューを設定
	dxUtility->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 参照バッファビューを設定
	dxUtility->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// マテリアルCBufferの場所を設定
	dxUtility->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// パーティクルのテクスチャパスを取得 (同じテクスチャのリストなので先頭だけでOK)
	std::string textureFullPath = particles.front().setting->textureFullPath;

	// SRVのDescriptorTableの先頭を設定
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetSRVGPUHandle(textureFullPath));

	/// === パーティクルCBufferの場所を設定 === ///
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager->GetGPUDescriptorHandle(srvIndex));

	// 描画(DrawCall)
	dxUtility->GetCommandList()->DrawIndexedInstanced(6, instanceCount, 0, 0, 0);
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

void PlaneRenderer::GenerateInstanceData() {

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

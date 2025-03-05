#include "Particle.h"
#include "ParticleCommon.h"
#include "base/SrvManager.h"
#include "2d/TextureManager.h"
#include "ModelManager.h"
#include "camera/Camera.h"
#include "math/MathVector.h"
#include "math/MathMatrix.h"

#include <numbers>
#include <random>
#include <imgui.h>

using namespace MathMatrix;

void Particle::Initialize(const std::string& directorypath, const std::string& filename) {

	// デフォルトカメラをセット
	this->camera = ParticleCommon::GetInstance()->GetDefaultCamera();

	InitializeShaderTransformData();

	// SRVを作成
	CreateSRVForTransformationMatrix();

	InitializeCameraData();

	// モデル読み込み
	modelData = ModelManager::LoadObjFile(directorypath, filename);

	// 頂点データ初期化
	InitializeVertexData();

	// マテリアルデータ初期化
	InitializeMaterialData();

	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);

	// 読み込んだテクスチャの番号を取得し、メンバ変数に書き込む
	modelData.material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textureFilePath);

	CreateRandom();
}

void Particle::Update() {

	numInstance = 0;

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {

		// 生存期間を過ぎていたら更新せず描画対象にしない
		if (lifeTime[index] <= currentTime[index]) {
			continue;
		}

		transform[index].translate += velocity[index] * kDeltaTime;

		currentTime[index] += kDeltaTime;

		// α値を下げる
		float alpha = 1.0f - (currentTime[index] / lifeTime[index]);

		/// === TransformからWorldMatrixを作る === ///
		Matrix4x4 worldMatrix = MakeAffineMatrix(transform[index].scale, transform[index].rotate, transform[index].translate);

		// WVP
		Matrix4x4 worldViewProjectionMatrix;

		// カメラがあればviewProjectionをもらってWVPの計算を行う
		if (camera) {

			const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();
			worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;

			// カメラのワールド座標を代入
			*cameraData = camera->GetWorldPosition();

			// カメラがなければworldMatrixを代入
		}
		else {

			worldViewProjectionMatrix = worldMatrix;
		}

		ShaderTransformData[index].WVP = worldViewProjectionMatrix;
		ShaderTransformData[index].world = worldMatrix;
		ShaderTransformData[index].color = color[index];
		ShaderTransformData[index].color.w = alpha;

		// 生きているParticleの数を1つカウントする
		++numInstance;
	}
}

void Particle::Draw() {

	/// === 座標変換行列CBufferの場所を設定 === ///
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(1, SrvManager::GetInstance()->GetGPUDescriptorHandle(srvIndex));

	/// === カメラCBufferの場所を設定 === ///
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, cameraResource->GetGPUVirtualAddress());

	// 頂点バッファビューを設定
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// マテリアルCBufferの場所を設定
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// SRVのDescriptorTableの先頭を設定
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVGPUHandle(modelData.material.textureFilePath));

	// 描画(DrawCall)
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), numInstance, 0, 0);
}

#include <string>

void Particle::ShowImGui(const char* name) {

	ImGui::Begin(name);

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {

		std::string namber = std::to_string(index);

		if (ImGui::TreeNode(namber.c_str())) {
			ImGui::DragFloat3("Scale", &transform[index].scale.x, 0.01f); // 大きさ
			ImGui::DragFloat3("Rotate", &transform[index].rotate.x, 0.01f); // 回転
			ImGui::DragFloat3("Translate", &transform[index].translate.x, 0.01f); // 位置
			ImGui::DragFloat("life", &lifeTime[index], 0.01f);
			ImGui::DragFloat("current", &currentTime[index], 0.01f);
			ImGui::DragFloat("Alpha", &ShaderTransformData[index].color.w, 0.01f);
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void Particle::CreateRandom() {

	// 乱数生成器
	std::random_device seedGenerator;

	std::mt19937 randomEngine(seedGenerator());

	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);

	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);

	// Transform変数を作る
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {

		transform[index].scale = { 1.0f,1.0f,1.0f };
		transform[index].rotate = { 0.0f,3.14f,0.0f };
		transform[index].translate = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };

		velocity[index] = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };

		color[index] = { distColor(randomEngine),distColor(randomEngine) ,distColor(randomEngine) ,1.0f };

		lifeTime[index] = distTime(randomEngine);
		currentTime[index] = 0.0f;
	}
}

void Particle::InitializeShaderTransformData() {

	/// === ShaderTransformResourceを作る === ///
	ShaderTransformResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(ShaderTransform) * kNumMaxInstance);

	/// === ShaderTransformResourceにデータを書き込むためのアドレスを取得してShaderTransformDataに割り当てる === ///
	ShaderTransformResource->Map(0, nullptr, reinterpret_cast<void**>(&ShaderTransformData));

	/// === ShaderTransformDataの初期値を書き込む === ///
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		ShaderTransformData[index].WVP = MakeIdentity4x4(); // 単位行列を書き込む
		ShaderTransformData[index].world = MakeIdentity4x4(); // 単位行列を書き込む
		ShaderTransformData[index].color = { 1.0f,1.0f,1.0f,1.0f }; // 白を書き込む
	}
}

void Particle::InitializeCameraData() {

	/// === CameraResource === ///
	cameraResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(Vector3));

	/// === CameraResourceにデータを書き込むためのアドレスを取得してCameraDataに割り当てる === ///
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));

	/// === CameraDataの初期値を書き込む === ///
	*cameraData = camera->GetWorldPosition();
}

void Particle::InitializeVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズ 頂点のサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	/// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// 頂点データにリソースをコピー
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void Particle::InitializeMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataの初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->lightingMode = 0; // Lightingをしていない
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
	materialData->shininess = 70.0f;
}

void Particle::CreateSRVForTransformationMatrix() {

	// SRVのインデックスを取得
	srvIndex = SrvManager::GetInstance()->Allocate();

	// 構造化バッファのリソース、要素数、ストライドを設定
	ID3D12Resource* pResource = ShaderTransformResource.Get();
	uint32_t numElements = kNumMaxInstance;
	UINT structureByteStride = sizeof(ShaderTransform);

	// SRVを作成
	SrvManager::GetInstance()->CreateSRVforStructuredBuffer(srvIndex, pResource, numElements, structureByteStride);
}

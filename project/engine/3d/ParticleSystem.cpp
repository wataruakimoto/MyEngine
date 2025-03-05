#include "ParticleSystem.h"
#include "ParticleCommon.h"
#include "base/SrvManager.h"
#include "2d/TextureManager.h"
#include "ModelManager.h"
#include "camera/Camera.h"
#include "math/MathVector.h"
#include "math/MathMatrix.h"

#include <string>
#include <numbers>
#include <imgui.h>

using namespace MathMatrix;

void ParticleSystem::Initialize(std::string textureFilePath) {

	this->textureFilePath = textureFilePath;

	// デフォルトカメラをセット
	this->camera = ParticleCommon::GetInstance()->GetDefaultCamera();

	InitializeParticleData();

	// SRVを作成
	CreateSRVForTransformationMatrix();

	// 頂点データ初期化
	InitializeVertexData();

	// 参照データ初期化
	InitializeIndexData();

	// マテリアルデータ初期化
	InitializeMaterialData();

	backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

	randomEngine.seed(seedGenerator());
}

void ParticleSystem::Update() {

	numInstance = 0;

	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, camera->GetWorldMatrix());
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	for (std::list<Particle>::iterator iterator = particles.begin(); iterator != particles.end();) {

		// 生存期間を過ぎていたら更新せず描画対象にしない
		if (iterator->lifeTime <= iterator->currentTime) {

			// Listから削除する
			iterator = particles.erase(iterator);
			continue;
		}

		iterator->transform.translate += iterator->velocity * kDeltaTime;

		iterator->currentTime += kDeltaTime;

		// α値を下げる
		float alpha = 1.0f - (iterator->currentTime / iterator->lifeTime);

		// Scaleのみの行列
		Matrix4x4 scaleMatrix = MakeScaleMatrix(iterator->transform.scale);

		// Translateのみの行列
		Matrix4x4 translateMatrix = MakeTranslateMatrix(iterator->transform.translate);

		/// === WorldMatrixを作る === ///
		Matrix4x4 worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;

		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();

		// WVP
		Matrix4x4 worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;

		if (numInstance < kNumMaxInstance) {

			// データに送る
			ParticleData[numInstance].WVP = worldViewProjectionMatrix;
			ParticleData[numInstance].world = worldMatrix;
			ParticleData[numInstance].color = iterator->color;
			ParticleData[numInstance].color.w = alpha;

			// 生きているParticleの数を1つカウントする
			++numInstance;
		}

		// 次のイテレーターに進める
		++iterator;
	}
}

void ParticleSystem::Draw() {

	/// === 座標変換行列CBufferの場所を設定 === ///
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(1, SrvManager::GetInstance()->GetGPUDescriptorHandle(srvIndex));

	// 頂点バッファビューを設定
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 参照バッファビューを設定
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// マテリアルCBufferの場所を設定
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// SRVのDescriptorTableの先頭を設定
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVGPUHandle(textureFilePath));

	// 描画(DrawCall)
	ParticleCommon::GetInstance()->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(6, numInstance, 0, 0, 0);
}

void ParticleSystem::ShowImGui(const char* name) {

	ImGui::Begin(name);

	if (ImGui::Button("Add Particle")) {
		particles.push_back(MakeNewParticle());
		particles.push_back(MakeNewParticle());
		particles.push_back(MakeNewParticle());
	}

	for (std::list<Particle>::iterator iterator = particles.begin(); iterator != particles.end(); ++iterator) {

		std::string namber = std::to_string(std::distance(particles.begin(), iterator));

		if (ImGui::TreeNode(namber.c_str())) {
			ImGui::DragFloat3("Scale", &iterator->transform.scale.x, 0.01f); // 大きさ
			ImGui::DragFloat3("Rotate", &iterator->transform.rotate.x, 0.01f); // 回転
			ImGui::DragFloat3("Translate", &iterator->transform.translate.x, 0.01f); // 位置
			ImGui::DragFloat("life", &iterator->lifeTime, 0.01f);
			ImGui::DragFloat("current", &iterator->currentTime, 0.01f);
			ImGui::DragFloat("Alpha", &iterator->color.w, 0.01f);
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

Particle ParticleSystem::MakeNewParticle() {

	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);

	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);

	Particle particle;

	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0.0f,3.14f,0.0f };
	particle.transform.translate = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };

	particle.velocity = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };

	particle.color = { distColor(randomEngine),distColor(randomEngine) ,distColor(randomEngine) ,1.0f };

	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0.0f;

	return particle;
}

std::list<Particle> ParticleSystem::Emit(const Emitter& emitter) {

	std::list<Particle> particles;

	for (uint32_t count = 0; count < emitter.count; ++count) {
		particles.push_back(MakeNewParticle());
	}

	return particles;
}

void ParticleSystem::InitializeParticleData() {

	/// === ShaderTransformResourceを作る === ///
	ParticleResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);

	/// === ShaderTransformResourceにデータを書き込むためのアドレスを取得してShaderTransformDataに割り当てる === ///
	ParticleResource->Map(0, nullptr, reinterpret_cast<void**>(&ParticleData));

	/// === ShaderTransformDataの初期値を書き込む === ///
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		ParticleData[index].WVP = MakeIdentity4x4(); // 単位行列を書き込む
		ParticleData[index].world = MakeIdentity4x4(); // 単位行列を書き込む
		ParticleData[index].color = { 1.0f,1.0f,1.0f,1.0f }; // 白を書き込む
	}
}

void ParticleSystem::InitializeVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 6);

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズ 頂点のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	/// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	/// === VertexResourceにデータを書き込む(4頂点) === ///

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

void ParticleSystem::InitializeIndexData() {

	/// === IndexResourceを作る === ///
	indexResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	/// === IBVを作成する(値を設定するだけ) === ///

	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	/// === IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる === ///
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	/// === IndexResourceにデータを書き込む(6個分)=== ///

	indexData[0] = 0; // 左下
	indexData[1] = 1; // 左上
	indexData[2] = 2; // 右下
	indexData[3] = 1; // 左上
	indexData[4] = 3; // 右上
	indexData[5] = 2; // 右下
}

void ParticleSystem::InitializeMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = ParticleCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataの初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}

void ParticleSystem::CreateSRVForTransformationMatrix() {

	// SRVのインデックスを取得
	srvIndex = SrvManager::GetInstance()->Allocate();

	// 構造化バッファのリソース、要素数、ストライドを設定
	ID3D12Resource* pResource = ParticleResource.Get();
	uint32_t numElements = kNumMaxInstance;
	UINT structureByteStride = sizeof(ParticleForGPU);

	// SRVを作成
	SrvManager::GetInstance()->CreateSRVforStructuredBuffer(srvIndex, pResource, numElements, structureByteStride);
}

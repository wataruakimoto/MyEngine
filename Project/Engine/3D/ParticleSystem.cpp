#include "ParticleSystem.h"
#include "ParticleCommon.h"
#include "base/DirectXUtility.h"
#include "base/SrvManager.h"
#include "2d/TextureManager.h"
#include "ModelManager.h"
#include "camera/Camera.h"
#include "math/MathVector.h"
#include "math/MathMatrix.h"
#include "debug/Logger.h"

#include <string>
#include <imgui.h>

using namespace MathMatrix;
using namespace Logger;

void ParticleSystem::Initialize() {

	// デフォルトカメラをセット
	this->camera = ParticleCommon::GetInstance()->GetDefaultCamera();

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

	// billboardMatrixの計算
	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, camera->GetWorldMatrix());
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	// ViewProjectionMatrixをカメラから取得
	const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();

	// 各パーティクルグループの更新
	for (auto& [key, particleGroup] : particleGroups) {

		// パーティクル数をリセット
		particleGroup.numInstance = 0;

		// グループ内のパーティクルを更新
		for (std::list<Particle>::iterator iterator = particleGroup.particles.begin(); iterator != particleGroup.particles.end();) {

			// 生存期間を過ぎていたら更新せず描画対象にしない
			if (iterator->lifeTime <= iterator->currentTime) {

				// Listから削除する
				iterator = particleGroup.particles.erase(iterator);
				continue;
			}

			// 移動処理
			iterator->transform.translate += iterator->velocity * kDeltaTime;
			iterator->currentTime += kDeltaTime;

			// α値を下げる
			float alpha = 1.0f - (iterator->currentTime / iterator->lifeTime);

			// Scaleのみの行列
			Matrix4x4 scaleMatrix = MakeScaleMatrix(iterator->transform.scale);
			// Rotateのみの行列
			Matrix4x4 rotateMatrix = MakeRotateMatrix(iterator->transform.rotate);
			// Translateのみの行列
			Matrix4x4 translateMatrix = MakeTranslateMatrix(iterator->transform.translate);

			// ビルボードするなら
			if (iterator->useBillboard) {

				// WorldMatrixを計算
				worldMatrix = scaleMatrix * rotateMatrix * billboardMatrix * translateMatrix;
			}
			// ビルボードしないなら
			else {

				// WorldMatrixを計算
				worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;
			}

			// WVPMatrixを合成
			worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;

			if (particleGroup.numInstance < kNumMaxInstance) {

				// デバッグ情報を出力
				if (particleGroup.particleData == nullptr) {
					Log("particleData is nullptr\n");
				}

				// データ１個分の書き込み
				particleGroup.particleData[particleGroup.numInstance].WVP = worldViewProjectionMatrix;
				particleGroup.particleData[particleGroup.numInstance].world = worldMatrix;
				particleGroup.particleData[particleGroup.numInstance].color = iterator->color;
				particleGroup.particleData[particleGroup.numInstance].color.w = alpha;

				// 生きているParticleの数を1つカウントする
				++particleGroup.numInstance;
			}

			// 次のイテレーターに進める
			++iterator;
		}
	}
}

void ParticleSystem::Draw() {

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

void ParticleSystem::Finalize() {

	delete instance;
	instance = nullptr;
}

void ParticleSystem::ShowImGui(const char* name) {

	ImGui::Begin(name);

	for (auto& [key, particleGroup] : particleGroups) {
		if (ImGui::TreeNode(key.c_str())) {
			int particleIndex = 0;
			for (const auto& particle : particleGroup.particles) {
				if (ImGui::TreeNode((key + " Particle " + std::to_string(particleIndex)).c_str())) {
					ImGui::Text("Scale: (%.2f, %.2f, %.2f)", particle.transform.scale.x, particle.transform.scale.y, particle.transform.scale.z);
					ImGui::Text("Rotate: (%.2f, %.2f, %.2f)", particle.transform.rotate.x, particle.transform.rotate.y, particle.transform.rotate.z);
					ImGui::Text("Translate: (%.2f, %.2f, %.2f)", particle.transform.translate.x, particle.transform.translate.y, particle.transform.translate.z);
					ImGui::Text("Velocity: (%.2f, %.2f, %.2f)", particle.velocity.x, particle.velocity.y, particle.velocity.z);
					ImGui::Text("Color: (%.2f, %.2f, %.2f, %.2f)", particle.color.x, particle.color.y, particle.color.z, particle.color.w);
					ImGui::Text("LifeTime: %.2f", particle.lifeTime);
					ImGui::Text("CurrentTime: %.2f", particle.currentTime);
					ImGui::TreePop();
				}
				++particleIndex;
			}
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void ParticleSystem::CreateParticleGroup(const std::string name, const std::string textureFilePath) {

	// 登録済みの名前かチェック
	if (particleGroups.find(name) != particleGroups.end()) {

		// 登録済みなら終了
		return;
	}

	// 新たなパーティクルグループの作成
	ParticleGroup particleGroup;

	// コンテナに登録
	particleGroups[name] = particleGroup;

	// テクスチャファイルパスを設定
	particleGroups[name].textureFilePath = textureFilePath;

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture(textureFilePath);

	// SRVインデックスを取得
	particleGroups[name].srvIndex = SrvManager::GetInstance()->Allocate();

	/// === ParticleResourceを作る === ///
	particleGroups[name].particleResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);

	/// === ParticleResourceにデータを書き込むためのアドレスを取得してParticleDataに割り当てる === ///
	particleGroups[name].particleResource->Map(0, nullptr, reinterpret_cast<void**>(&particleGroups[name].particleData));

	/// === ParticleDataの初期値を書き込む === ///
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		particleGroups[name].particleData[index].WVP = MakeIdentity4x4(); // 単位行列を書き込む
		particleGroups[name].particleData[index].world = MakeIdentity4x4(); // 単位行列を書き込む
		particleGroups[name].particleData[index].color = { 1.0f,1.0f,1.0f,1.0f }; // 白を書き込む
	}

	// SRV生成
	SrvManager::GetInstance()->CreateSRVforStructuredBuffer(particleGroups[name].srvIndex, particleGroups[name].particleResource.Get(), kNumMaxInstance, sizeof(ParticleForGPU));
}

void ParticleSystem::Emit(const std::string name, const Vector3& position, uint32_t count, Particle setting) {

	// 登録済みのParticleGroupかチェック
	if (particleGroups.find(name) == particleGroups.end()) {

		// 登録されていないならエラーメッセージを出す
		Log("Not found ParticleGroup: " + name + "\n");
		return;
	}

	// 新しいパーティクルを作成
	for (uint32_t index = 0; index < count; ++index) {

		// 新しいパーティクルを作成
		Particle particle = MakeNewParticle(setting);

		// 位置を設定
		particle.transform.translate = position;

		// 指定されたパーティクルグループに追加
		particleGroups[name].particles.push_back(particle);
	}
}

void ParticleSystem::InitializeVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(VertexData) * 6);

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
	materialResource = ParticleCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してMaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataの初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 今は白を書き込んでいる
	materialData->uvTransform = MakeIdentity4x4(); // 単位行列で初期化
}

Particle ParticleSystem::MakeNewParticle(Particle setting) {

	// 返す用のパーティクル
	Particle resultParticle;
	
	// Scaleの設定
	if (setting.randomizeScale) {

		// X軸のスケールをランダムに設定
		std::uniform_real_distribution<float> distributionX(setting.randomScaleMin.x, setting.randomScaleMax.x);
		resultParticle.transform.scale.x = distributionX(randomEngine);

		// Y軸のスケールをランダムに設定
		std::uniform_real_distribution<float> distributionY(setting.randomScaleMin.y, setting.randomScaleMax.y);
		resultParticle.transform.scale.y = distributionY(randomEngine);

		// Z軸のスケールをランダムに設定
		std::uniform_real_distribution<float> distributionZ(setting.randomScaleMin.z, setting.randomScaleMax.z);
		resultParticle.transform.scale.z = distributionZ(randomEngine);
	}
	else {

		// 設定項目を引き継ぐ
		resultParticle.transform.scale = setting.transform.scale;
	}

	// Rotateの設定
	if (setting.randomizeRotate) {

		// X軸の回転をランダムに設定
		std::uniform_real_distribution<float> distributionX(setting.randomRotateMin.x, setting.randomRotateMax.x);
		resultParticle.transform.rotate.x = distributionX(randomEngine);

		// Y軸の回転をランダムに設定
		std::uniform_real_distribution<float> distributionY(setting.randomRotateMin.y, setting.randomRotateMax.y);
		resultParticle.transform.rotate.y = distributionY(randomEngine);

		// Z軸の回転をランダムに設定
		std::uniform_real_distribution<float> distributionZ(setting.randomRotateMin.z, setting.randomRotateMax.z);
		resultParticle.transform.rotate.z = distributionZ(randomEngine);
	}
	else {

		// 設定項目を引き継ぐ
		resultParticle.transform.rotate = setting.transform.rotate;
	}

	// Translateの設定
	if (setting.randomizeTranslate) {

		// X軸の位置をランダムに設定
		std::uniform_real_distribution<float> distributionX(setting.randomTranslateMin.x, setting.randomTranslateMax.x);
		resultParticle.transform.translate.x = distributionX(randomEngine);

		// Y軸の位置をランダムに設定
		std::uniform_real_distribution<float> distributionY(setting.randomTranslateMin.y, setting.randomTranslateMax.y);
		resultParticle.transform.translate.y = distributionY(randomEngine);

		// Z軸の位置をランダムに設定
		std::uniform_real_distribution<float> distributionZ(setting.randomTranslateMin.z, setting.randomTranslateMax.z);
		resultParticle.transform.translate.z = distributionZ(randomEngine);
	}
	else {
		
		// 設定項目を引き継ぐ
		resultParticle.transform.translate = setting.transform.translate;
	}

	// Velocityの設定
	if (setting.randomizeVelocity) {
		
		// X軸の速度をランダムに設定
		std::uniform_real_distribution<float> distributionX(setting.randomVelocityMin.x, setting.randomVelocityMax.x);
		resultParticle.velocity.x = distributionX(randomEngine);
		
		// Y軸の速度をランダムに設定
		std::uniform_real_distribution<float> distributionY(setting.randomVelocityMin.y, setting.randomVelocityMax.y);
		resultParticle.velocity.y = distributionY(randomEngine);
		
		// Z軸の速度をランダムに設定
		std::uniform_real_distribution<float> distributionZ(setting.randomVelocityMin.z, setting.randomVelocityMax.z);
		resultParticle.velocity.z = distributionZ(randomEngine);
	}
	else {
		
		// 設定項目を引き継ぐ
		resultParticle.velocity = setting.velocity;
	}

	// Colorの設定
	if (setting.randomizeColor) {

		// Rの色をランダムに設定
		std::uniform_real_distribution<float> distributionR(setting.randomColorMin.x, setting.randomColorMax.x);
		resultParticle.color.x = distributionR(randomEngine);

		// Gの色をランダムに設定
		std::uniform_real_distribution<float> distributionG(setting.randomColorMin.y, setting.randomColorMax.y);
		resultParticle.color.y = distributionG(randomEngine);

		// Bの色をランダムに設定
		std::uniform_real_distribution<float> distributionB(setting.randomColorMin.z, setting.randomColorMax.z);
		resultParticle.color.z = distributionB(randomEngine);

		// Aの色をランダムに設定
		std::uniform_real_distribution<float> distributionA(setting.randomColorMin.w, setting.randomColorMax.w);
		resultParticle.color.w = distributionA(randomEngine);
	}
	else {
		
		// 設定項目を引き継ぐ
		resultParticle.color = setting.color;
	}

	// LifeTimeの設定
	if (setting.randomizeLifeTime) {

		// 寿命をランダムに設定
		std::uniform_real_distribution<float> distribution(setting.randomLifeTimeMin, setting.randomLifeTimeMax);
		resultParticle.lifeTime = distribution(randomEngine);
	}
	else {

		// 設定項目を引き継ぐ
		resultParticle.lifeTime = setting.lifeTime;
	}

	resultParticle.currentTime = 0.0f;

	resultParticle.useBillboard = setting.useBillboard;

	return resultParticle;
}

ParticleSystem* ParticleSystem::instance = nullptr;

ParticleSystem* ParticleSystem::GetInstance() {

	if (instance == nullptr) {
		instance = new ParticleSystem;
	}

	return instance;
}

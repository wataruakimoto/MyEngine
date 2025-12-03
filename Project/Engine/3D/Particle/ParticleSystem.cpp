#include "ParticleSystem.h"
#include "ParticleCommon.h"
#include "PlaneParticle.h"
#include "RingParticle.h"
#include "CylinderParticle.h"
#include "CubeParticle.h"
#include "SpherePariticle.h"
#include "ShardParticle.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "Texture/TextureManager.h"
#include "Model/ModelManager.h"
#include "Camera.h"
#include "MathVector.h"
#include "MathMatrix.h"
#include "Logger.h"

#include <string>
#include <imgui.h>

using namespace MathVector;
using namespace MathMatrix;
using namespace Logger;

void ParticleSystem::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// デフォルトカメラをセット
	this->camera = ParticleCommon::GetInstance()->GetDefaultCamera();

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

			if (iterator->useLifeTime) {
				iterator->currentTime += kDeltaTime;
			}

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

		switch (particleGroup.particleType) {

		case ParticleType::PLANE:
		default:
			particleGroup.particleTypeClass->Update();
			break;
	
		case ParticleType::RING:
			particleGroup.particleTypeClass->Update();
			break;

		case ParticleType::CYLINDER:
			particleGroup.particleTypeClass->Update();
			break;

		case ParticleType::CUBE:
			particleGroup.particleTypeClass->Update();
			break;

		case ParticleType::SPHERE:
			particleGroup.particleTypeClass->Update();
			break;

		case ParticleType::SHARD:
			particleGroup.particleTypeClass->Update();
			break;
		}
	}
}

void ParticleSystem::Draw() {

	// 各パーティクルグループの描画
	for (auto& [key, particleGroup] : particleGroups) {

		// インスタンスが0のとき
		if (particleGroup.numInstance == 0) {

			// 描画処理に入らず次にSkip
			continue;
		}

		switch (particleGroup.particleType) {

		case ParticleType::PLANE:
		default:
			particleGroup.particleTypeClass->Draw(&particleGroup);
			break;

		case ParticleType::RING:
			particleGroup.particleTypeClass->Draw(&particleGroup);
			break;

		case ParticleType::CYLINDER:
			particleGroup.particleTypeClass->Draw(&particleGroup);
			break;

		case ParticleType::CUBE:
			particleGroup.particleTypeClass->Draw(&particleGroup);
			break;

		case ParticleType::SPHERE:
			particleGroup.particleTypeClass->Draw(&particleGroup);
			break;

		case ParticleType::SHARD:
			particleGroup.particleTypeClass->Draw(&particleGroup);
			break;
		}
	}
}

void ParticleSystem::Finalize() {

	// 各パーティクルグループのタイプクラスの破棄
	for (auto& [key, particleGroup] : particleGroups) {

		delete particleGroup.particleTypeClass;
	}

	delete instance;
	instance = nullptr;
}

void ParticleSystem::ShowImGui(const char* name) {

#ifdef USE_IMGUI

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

#endif // USE_IMGUI
}

void ParticleSystem::CreateParticleGroup(const std::string name, const std::string textureFilePath, ParticleType type) {

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
	particleGroups[name].particleResource = dxUtility->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);

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

	// パーティクルの種類を設定
	particleGroups[name].particleType = type;

	// パーティクルの種類に応じたクラスを生成
	switch (type) {
	case ParticleType::PLANE:
	default:
		particleGroups[name].particleTypeClass = new PlaneParticle();
		break;

	case ParticleType::RING:
		particleGroups[name].particleTypeClass = new RingParticle();
		break;

	case ParticleType::CYLINDER:
		particleGroups[name].particleTypeClass = new CylinderParticle();
		break;

	case ParticleType::CUBE:
		particleGroups[name].particleTypeClass = new CubeParticle();
		break;

	case ParticleType::SPHERE:
		particleGroups[name].particleTypeClass = new SpherePariticle();
		break;

	case ParticleType::SHARD:
		particleGroups[name].particleTypeClass = new ShardParticle();
		break;
	}

	// パーティクルの種類を初期化
	particleGroups[name].particleTypeClass->Initialize();
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

void ParticleSystem::EmitExplosion(const std::string name, const Vector3& center, float radius, uint32_t count, Particle setting) {

	// 登録済みのParticleGroupかチェック
	if (particleGroups.find(name) == particleGroups.end()) {

		// 登録されていないならエラーメッセージを出す
		Log("Not found ParticleGroup: " + name + "\n");
		return;
	}

	// ランダム生成器の準備（-1.0 ～ 1.0 の範囲）
	std::uniform_real_distribution<float> distDir(-1.0f, 1.0f);
	// 速度の強さをランダムにする（settingのVelocityMin.x ～ Max.x を「速さ」として使う例）
	std::uniform_real_distribution<float> distSpeed(setting.randomVelocityMin.x, setting.randomVelocityMax.x);

	for (uint32_t i = 0; i < count; ++i) {

		// 基本設定でパーティクル作成（色や寿命などは引き継ぐ）
		Particle particle = MakeNewParticle(setting);

		// 1. ランダムな方向ベクトルを作成
		Vector3 direction = { distDir(randomEngine), distDir(randomEngine), distDir(randomEngine) };

		// 正規化（長さを1にする） ※MathVectorのNormalize関数を使用
		direction = Normalize(direction);

		// 2. 位置の決定：球の表面（または内部）に配置
		// center + (方向 × 半径)
		particle.transform.translate = center + (direction * radius);

		// 3. 速度の決定：中心から外側に向かって飛ばす
		float speed = distSpeed(randomEngine); // ランダムな速さ
		particle.velocity = direction * speed; // 方向 × 速さ

		// リストに追加
		particleGroups[name].particles.push_back(particle);
	}
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

	resultParticle.useLifeTime = setting.useLifeTime;

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

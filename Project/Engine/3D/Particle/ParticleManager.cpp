#include "ParticleManager.h"
#include "Texture/TextureManager.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "Camera.h"
#include "Model/ModelManager.h"
#include "MathVector.h"
#include "MathMatrix.h"

#include <numbers>
#include <fstream>
#include <cassert>
#include <imgui.h>

using namespace Engine;
using namespace MathVector;
using namespace MathMatrix;
using namespace MathRandom;
using namespace nlohmann;

void ParticleManager::Initialize() {

	// テクスチャマネージャーのインスタンスを取得
	textureManager = TextureManager::GetInstance();

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// SRVマネージャーのインスタンスを取得
	srvManager = SrvManager::GetInstance();

	// モデルマネージャのインスタンスを取得
	modelManager_ = ModelManager::GetInstance();

	// JSONからパーティクル設定を全て読み込み
	LoadParticleSettingsFromJSON();

	// 板ポリのレンダラーを作成
	planeRenderer = std::make_unique<PlaneRenderer>();
	// 板ポリのレンダラーを初期化
	planeRenderer->Initialize();

	// リングのレンダラーを作成
	ringRenderer = std::make_unique<RingRenderer>();
	// リングのレンダラーを初期化
	ringRenderer->Initialize();

	// シリンダーのレンダラーを作成
	cylinderRenderer = std::make_unique<CylinderRenderer>();
	// シリンダーのレンダラーを初期化
	cylinderRenderer->Initialize();

	// キューブのレンダラーを作成
	cubeRenderer = std::make_unique<CubeRenderer>();
	// キューブのレンダラーを初期化
	cubeRenderer->Initialize();

	// シャードのレンダラーを作成
	shardRenderer = std::make_unique<ShardRenderer>();
	// シャードのレンダラーを初期化
	shardRenderer->Initialize();

	// メッシュのレンダラーを作成
	meshRenderer = std::make_unique<MeshRenderer>();
	// メッシュのレンダラーを初期化
	meshRenderer->Initialize();
}

void ParticleManager::Update() {

	// カメラからViewProjectionを受け取る
	viewProjectionMatrix = camera->GetViewProjectionMatrix();

	// 180度回転行列を作成
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

	// ビルボード行列を計算
	billboardMatrix = backToFrontMatrix * camera->GetWorldTransform().GetWorldMatrix();

	// 行列の平行移動成分を排除する
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	// 板ポリのパーティクルコンテナの更新
	UpdateGroups(planeGroups);

	// リングのパーティクルコンテナの更新
	UpdateGroups(ringGroups);

	// シリンダーのパーティクルコンテナの更新
	UpdateGroups(cylinderGroups);

	// キューブのパーティクルコンテナの更新
	UpdateGroups(cubeGroups);

	// シャードのパーティクルコンテナの更新
	UpdateGroups(shardGroups);

	// メッシュのパーティクルコンテナの更新
	UpdateGroups(meshGroups);
}

void ParticleManager::Draw() {

	// カメラがないと描画できないのでアサート
	assert(camera && "Camera is nullptr");

	// 板ポリのパーティクルコンテナの描画
	for (auto& [effectName, group] : planeGroups) {

		// リストが空なら描画しない
		if (group.particles.empty() || group.numInstance <= 0) continue;

		// 板ポリレンダラーで描画
		planeRenderer->Draw(group.numInstance, group.srvIndex, settings[effectName].textureFullPath);
	}

	// リングのパーティクルコンテナの描画
	for (auto& [effectName, group] : ringGroups) {

		// リストが空なら描画しない
		if (group.particles.empty() || group.numInstance <= 0) continue;

		// リングレンダラーで描画
		ringRenderer->Draw(group.numInstance, group.srvIndex, settings[effectName].textureFullPath);
	}

	// シリンダーのパーティクルコンテナの描画
	for (auto& [effectName, group] : cylinderGroups) {

		// リストが空なら描画しない
		if (group.particles.empty() || group.numInstance <= 0) continue;

		// シリンダーレンダラーで描画
		cylinderRenderer->Draw(group.numInstance, group.srvIndex, settings[effectName].textureFullPath);
	}

	// キューブのパーティクルコンテナの描画
	for (auto& [effectName, group] : cubeGroups) {

		// リストが空なら描画しない
		if (group.particles.empty() || group.numInstance <= 0) continue;

		// キューブレンダラーで描画
		cubeRenderer->Draw(group.numInstance, group.srvIndex, settings[effectName].textureFullPath);
	}

	// シャードのパーティクルコンテナの描画
	for (auto& [effectName, group] : shardGroups) {

		// リストが空なら描画しない
		if (group.particles.empty() || group.numInstance <= 0) continue;

		// シャードレンダラーで描画
		shardRenderer->Draw(group.numInstance, group.srvIndex, settings[effectName].textureFullPath);
	}

	// メッシュのパーティクルコンテナの描画
	for (auto& [effectName, group] : meshGroups) {

		// リストが空なら描画しない
		if (group.particles.empty() || group.numInstance <= 0) continue;

		// メッシュレンダラーで描画
		meshRenderer->Draw(group.numInstance, group.srvIndex, settings[effectName].textureFullPath);
	}
}

void ParticleManager::Finalize() {

	// インスタンスの破棄
	delete instance;

	// インスタンスにnullptr代入
	instance = nullptr;
}

void ParticleManager::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("Particle Manager");

	// ==========================================
	// 1. 既存エフェクトの選択
	// ==========================================
	if (ImGui::BeginCombo("Select Effect", currentEditName.c_str())) {
		for (auto& [name, setting] : settings) {
			bool isSelected = (currentEditName == name);
			if (ImGui::Selectable(name.c_str(), isSelected)) {
				currentEditName = name;
				tempSetting = setting; // 選択した設定を編集用バッファにコピー
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();

	// ==========================================
	// 2. 新規エフェクトの作成
	// ==========================================
	ImGui::InputText("New Effect Name", inputNameBuffer, sizeof(inputNameBuffer));
	ImGui::SameLine();
	if (ImGui::Button("Create New")) {
		std::string newName = inputNameBuffer;
		// 名前が空でなく、まだ存在しない名前なら作成
		if (!newName.empty() && settings.find(newName) == settings.end()) {
			ParticleSetting newSetting;
			newSetting.effectName = newName;
			settings[newName] = newSetting;

			// 作成したものをすぐに選択状態にする
			currentEditName = newName;
			tempSetting = newSetting;
		}
	}

	ImGui::Separator();

	// ==========================================
	// 3. パラメータの編集
	// ==========================================
	if (!currentEditName.empty()) {
		ImGui::Text("Editing: %s", currentEditName.c_str());

		// --- 形状の編集 ---
		const char* shapeNames[] = { "PLANE", "RING", "CYLINDER", "CUBE", "SPHERE", "SHARD", "MESH" };
		int shapeIndex = static_cast<int>(tempSetting.shape);
		if (ImGui::Combo("Shape", &shapeIndex, shapeNames, IM_ARRAYSIZE(shapeNames))) {
			tempSetting.shape = static_cast<ParticleShape>(shapeIndex);
		}

		// ビルボードフラグ
		ImGui::Checkbox("Use Billboard", &tempSetting.useBillboard);

		// --- 寿命の設定 ---
		if (ImGui::TreeNode("LifeTime")) {
			ImGui::Checkbox("Random##Life", &tempSetting.lifeTimeRandom);
			if (tempSetting.lifeTimeRandom) {
				ImGui::DragFloat("Min##Life", &tempSetting.lifeTimeRange.min, 0.01f, 0.0f, 10.0f);
				ImGui::DragFloat("Max##Life", &tempSetting.lifeTimeRange.max, 0.01f, 0.0f, 10.0f);
			}
			else {
				ImGui::DragFloat("Value##Life", &tempSetting.lifeTime, 0.01f, 0.0f, 10.0f);
			}
			ImGui::TreePop();
		}

		// --- スケールの設定 ---
		if (ImGui::TreeNode("Scale")) {
			ImGui::Checkbox("Random##Scale", &tempSetting.scaleRandom);
			if (tempSetting.scaleRandom) {
				ImGui::DragFloat3("Min##Scale", &tempSetting.scaleRange.min.x, 0.01f);
				ImGui::DragFloat3("Max##Scale", &tempSetting.scaleRange.max.x, 0.01f);
			}
			else {
				ImGui::DragFloat3("Value##Scale", &tempSetting.scale.x, 0.01f);
			}
			ImGui::TreePop();
		}

		// --- 速度の設定 ---
		if (ImGui::TreeNode("Velocity")) {
			ImGui::Checkbox("Random##Vel", &tempSetting.velocityRandom);
			if (tempSetting.velocityRandom) {
				ImGui::DragFloat3("Min##Vel", &tempSetting.velocityRange.min.x, 0.01f);
				ImGui::DragFloat3("Max##Vel", &tempSetting.velocityRange.max.x, 0.01f);
			}
			else {
				ImGui::DragFloat3("Value##Vel", &tempSetting.velocity.x, 0.01f);
			}
			ImGui::TreePop();
		}

		// ※ Rotate, Translate, Acceleration, Color なども同様に追加します

		ImGui::Separator();

		// ==========================================
		// 4. 適用と保存
		// ==========================================
		if (ImGui::Button("Apply & Save JSON")) {
			// 一時バッファ(tempSetting)の内容を本来のマップに反映
			settings[currentEditName] = tempSetting;

			// すでに用意されているJSON保存関数を呼び出す
			SaveSettingsToJSON(currentEditName);
		}
	}

	ImGui::End();

#endif // USE_IMGUI
}

void ParticleManager::AddSetting(ParticleSetting& setting) {

	// 設定を追加
	settings[setting.effectName] = setting;
}

void ParticleManager::AddInstance(const ParticleInstance& instance) {

	// エフェクト名を取得
	std::string key = instance.setting->effectName;

	// 形状で分岐
	switch (instance.setting->shape) {

	case ParticleShape::PLANE:

		// リソース未作成なら
		if (!planeGroups[key].isResourceCreated) {

			// リソース作成
			CreateGroupResource(planeGroups[key]);
		}

		// グループのリストに追加
		planeGroups[key].particles.push_back(instance);

		break;

	case ParticleShape::RING:

		// リソース未作成なら
		if (!ringGroups[key].isResourceCreated) {
			// リソース作成
			CreateGroupResource(ringGroups[key]);
		}

		// グループのリストに追加
		ringGroups[key].particles.push_back(instance);
		break;

	case ParticleShape::CYLINDER:

		// リソース未作成なら
		if (!cylinderGroups[key].isResourceCreated) {

			// リソース作成
			CreateGroupResource(cylinderGroups[key]);
		}

		// グループのリストに追加
		cylinderGroups[key].particles.push_back(instance);

		break;

	case ParticleShape::CUBE:

		// リソース未作成なら
		if (!cubeGroups[key].isResourceCreated) {

			// リソース作成
			CreateGroupResource(cubeGroups[key]);
		}

		// グループのリストに追加
		cubeGroups[key].particles.push_back(instance);

		break;

	case ParticleShape::SHARD:

		// リソース未作成なら
		if (!shardGroups[key].isResourceCreated) {

			// リソース作成
			CreateGroupResource(shardGroups[key]);
		}

		// グループのリストに追加
		shardGroups[key].particles.push_back(instance);

		break;

	case ParticleShape::MESH:

		// リソース未作成なら
		if (!meshGroups[key].isResourceCreated) {

			// リソース作成
			CreateGroupResource(meshGroups[key]);
		}

		// グループのリストに追加
		meshGroups[key].particles.push_back(instance);

		break;
	}
}

void ParticleManager::Clear() {

	// 板ポリのパーティクルをクリア
	for (auto& [key, group] : planeGroups) {
		group.particles.clear();
	}

	// リングのパーティクルをクリア
	for (auto& [key, group] : ringGroups) {
		group.particles.clear();
	}

	// シリンダーのパーティクルをクリア
	for (auto& [key, group] : cylinderGroups) {
		group.particles.clear();
	}

	// キューブのパーティクルをクリア
	for (auto& [key, group] : cubeGroups) {
		group.particles.clear();
	}

	// シャードのパーティクルをクリア
	for (auto& [key, group] : shardGroups) {
		group.particles.clear();
	}

	// メッシュのパーティクルをクリア
	for (auto& [key, group] : meshGroups) {
		group.particles.clear();
	}
}

void ParticleManager::UpdateParticles(std::list<ParticleInstance>& particles) {

	// 全パーティクルの更新
	for (auto ite = particles.begin(); ite != particles.end(); ) {

		// 時間経過
		ite->currentTime += kDeltaTime;

		// 寿命が来ていたら
		if (ite->currentTime >= ite->lifeTime) {
			// リストから削除
			ite = particles.erase(ite);
			// 次のパーティクルへ
			continue;
		}

		// 0.0f(生まれたて) -> 1.0f(死ぬ直前)
		float alphaRatio = 1.0f - (ite->currentTime / ite->lifeTime);

		// アルファ値を更新
		ite->color.w = alphaRatio;

		// 加速度による速度変化
		ite->velocity += ite->acceleration;
		// 速度による位置変化
		ite->translate += ite->velocity;
		// 次のパーティクルへ
		++ite;
	}
}

void ParticleManager::UpdateGroups(std::unordered_map<std::string, ParticleGroup>& groups) {

	for (auto& [key, group] : groups) {

		// インスタンス数をリセット
		group.numInstance = 0;

		if (group.particles.empty()) continue;

		// 各パーティクルの更新
		UpdateParticles(group.particles);

		for (const auto& particle : group.particles) {

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
			group.instanceData[group.numInstance].world = worldMatrix;
			group.instanceData[group.numInstance].WVP = wvpMatrix;
			group.instanceData[group.numInstance].color = particle.color;

			// インスタンス数をインクリメント
			group.numInstance++;
		}
	}
}

void ParticleManager::LoadParticleSettingsFromJSON() {

	// ディレクトリ内の全jsonを走査して読み込む
	if (!std::filesystem::exists(kDataFolderPath)) {
		std::filesystem::create_directories(kDataFolderPath);
		return; // まだフォルダがない
	}

	for (const auto& entry : std::filesystem::directory_iterator(kDataFolderPath)) {
		if (entry.path().extension() == ".json") {
			LoadSettingsFromJSON(entry.path().string());
		}
	}
}

void ParticleManager::SaveSettingsToJSON(const std::string& effectName) {

	// 設定が存在するかチェック
	if (settings.find(effectName) == settings.end()) {
		assert(false && "Particle Setting not found for saving");
		return;
	}

	// ディレクトリ確認
	if (!std::filesystem::exists(kDataFolderPath)) {
		std::filesystem::create_directories(kDataFolderPath);
	}

	// JSONに変換して保存
	json j = settings[effectName];
	std::string path = kDataFolderPath + effectName + ".json";

	std::ofstream o(path);
	o << std::setw(4) << j << std::endl;
	o.close();
}

void ParticleManager::LoadSettingsFromJSON(const std::string& filePath) {

	std::ifstream i(filePath);
	if (!i.is_open()) return;

	json j;
	i >> j;

	ParticleSetting setting;
	setting = j; // JSON -> 構造体へ変換

	/// ========== テクスチャの読み込み ========== ///

	// テクスチャファイル名が空でないなら
	if (!setting.textureFileName.empty()) {

		// テクスチャのベースディレクトリパスを取得
		const std::string textureBasePath = textureManager->GetBaseDirectoryPath();

		// テクスチャのフルパスの作成
		std::filesystem::path textureFullPath = std::filesystem::path(textureBasePath) / "Particles" / setting.textureFileName;

		// テクスチャの読み込み
		textureManager->LoadTextureFullPath(textureFullPath.generic_string());
	}

	/// ========== モデルの読み込み ========== ///

	// モデルファイル名が空でないなら
	if (!setting.modelFileName.empty()) {

		// モデルのベースディレクトリパスを取得
		const std::string modelBasePath = modelManager_->GetBaseDirectoryPath();

		// モデルのフルパスを作成
		std::filesystem::path modelFullPath = std::filesystem::path(modelBasePath) / "Particles" / setting.modelFileName;

		// モデルの読み込み
		modelManager_->LoadModelFullPath(modelFullPath.generic_string());
	}

	// マップに登録
	settings[setting.effectName] = setting;
}

void ParticleManager::CreateGroupResource(ParticleGroup& group) {

	// 既に作成済みなら何もしない
	if (group.isResourceCreated) return;

	/// === InstanceResourceを作る === ///
	group.instanceResource = dxUtility->CreateBufferResource(sizeof(InstanceData) * group.kMaxInstanceCount);

	/// === InstanceResourceにデータを書き込むためのアドレスを取得してInstanceDataに割り当てる === ///
	group.instanceResource->Map(0, nullptr, reinterpret_cast<void**>(&group.instanceData));

	/// === InstanceDataに初期値を書き込む === ///
	for (uint32_t index = 0; index < group.kMaxInstanceCount; ++index) {

		group.instanceData[index].WVP = MakeIdentity4x4(); // 単位行列を書き込む
		group.instanceData[index].world = MakeIdentity4x4(); // 単位行列を書き込む
		group.instanceData[index].color = { 1.0f,1.0f,1.0f,1.0f }; // 白を書き込む
	}

	// SRVインデックスを取得
	group.srvIndex = srvManager->Allocate();

	/// === SRVを作成 === ///
	srvManager->CreateSRVforStructuredBuffer(group.srvIndex, group.instanceResource.Get(), group.kMaxInstanceCount, sizeof(InstanceData));

	// リソースを作成済みにしておく
	group.isResourceCreated = true;
}

ParticleManager* ParticleManager::instance = nullptr;

ParticleManager* ParticleManager::GetInstance() {

	if (instance == nullptr) {

		instance = new ParticleManager();
	}

	return instance;
}

ParticleSetting* ParticleManager::GetSetting(const std::string& effectName) {

	// 名前が存在するかチェック
	if (settings.find(effectName) != settings.end()) {

		// 見つかったらその設定を返す
		return &settings[effectName];
	}

	// 見つからなかったらメッセージを出してnullptrを返す
	assert(false && "Particle Setting not found");
	return nullptr;
}

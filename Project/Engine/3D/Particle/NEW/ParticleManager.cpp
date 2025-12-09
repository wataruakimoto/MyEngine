#include "ParticleManager.h"
#include "Texture/TextureManager.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "Camera.h"
#include "MathVector.h"
#include "MathMatrix.h"

#include <numbers>
#include <fstream>
#include <cassert>
#include <imgui.h>

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

	// JSONからパーティクル設定を全て読み込み
	LoadParticleSettingsFromJSON();
}

void ParticleManager::Update() {

	// カメラからViewProjectionを受け取る
	viewProjectionMatrix = camera->GetViewProjectionMatrix();

	// 180度回転行列を作成
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

	// ビルボード行列を計算
	billboardMatrix = backToFrontMatrix * camera->GetWorldMatrix();

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
}

void ParticleManager::Draw() {

	// カメラがないと描画できないのでアサート
	assert(camera && "Camera is nullptr");

	// 板ポリのパーティクルコンテナの描画
	for (auto& [textureFileName, group] : planeGroups) {

		// リストが空なら描画しない
		if (group.particles.empty()) continue;

		// 板ポリレンダラーで描画
		planeRenderer->Draw(group.numInstance, group.srvIndex, textureFileName);
	}

	// リングのパーティクルコンテナの描画
	for (auto& [textureFileName, group] : ringGroups) {
		// リストが空なら描画しない
		if (group.particles.empty()) continue;
		// リングレンダラーで描画
		ringRenderer->Draw(group.numInstance, group.srvIndex, textureFileName);
	}

	// シリンダーのパーティクルコンテナの描画
	for (auto& [textureFileName, group] : cylinderGroups) {
		// リストが空なら描画しない
		if (group.particles.empty()) continue;
		// シリンダーレンダラーで描画
		cylinderRenderer->Draw(group.numInstance, group.srvIndex, textureFileName);
	}

	// キューブのパーティクルコンテナの描画
	for (auto& [textureFileName, group] : cubeGroups) {

		// リストが空なら描画しない
		if (group.particles.empty()) continue;

		// キューブレンダラーで描画
		cubeRenderer->Draw(group.numInstance, group.srvIndex, textureFileName);
	}

	// シャードのパーティクルコンテナの描画
	for (auto& [textureFileName, group] : shardGroups) {
		// リストが空なら描画しない
		if (group.particles.empty()) continue;
		// シャードレンダラーで描画
		shardRenderer->Draw(group.numInstance, group.srvIndex, textureFileName);
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

	// 画面を左右に分割
	ImGui::Columns(2, "ParticleEditorCols", true);

	// 初回のみ左側の幅を固定（例: 200px）
	static bool firstTime = true;
	if (firstTime) {
		ImGui::SetColumnWidth(0, 240.0f);
		firstTime = false;
	}

	// ============================================================
	// 【左側】 エフェクトリスト
	// ============================================================

	// ShowEffectList(); // 関数化している場合はこれを呼ぶだけ

	// または直接書くなら以下のように BeginChild を使うと
	// リスト部分だけスクロールできてエディタっぽくなります
	ImGui::BeginChild("EffectListPanel", ImVec2(0, 0), true);
	ShowEffectList();
	ImGui::EndChild();

	// ============================================================
	// 次のカラム（右側）へ移動
	// ============================================================
	ImGui::NextColumn();

	// ============================================================
	// 【右側】 パラメータ詳細
	// ============================================================

	// 右側もスクロールできるようにChildで囲むのがおすすめ
	ImGui::BeginChild("ParameterPanel", ImVec2(0, 0), false);
	ShowParameters();
	ImGui::EndChild();

	// ============================================================
	// カラム設定を解除（1カラムに戻す）
	// ============================================================
	ImGui::Columns(1);

	ImGui::End();

	ImGui::Begin("Particle Instances");

	// グループコンテナのリスト
	struct GroupContainerInfo {
		const char* name;
		std::unordered_map<std::string, ParticleGroupNew>* container;
	} containers[] = {
		{ "Plane", &planeGroups },
		{ "Ring", &ringGroups },
		{ "Cylinder", &cylinderGroups },
		{ "Cube", &cubeGroups },
		{ "Shard", &shardGroups },
	};

	for (const auto& containerInfo : containers) {
		if (ImGui::TreeNode(containerInfo.name)) {
			for (const auto& [textureKey, group] : *containerInfo.container) {
				std::string groupLabel = textureKey + " (" + std::to_string(group.particles.size()) + " instances)";
				if (ImGui::TreeNodeEx(groupLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
					int idx = 0;
					for (const auto& inst : group.particles) {
						std::string instLabel = "Instance " + std::to_string(idx);
						if (ImGui::TreeNodeEx(instLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
							ImGui::Text("Scale:      (%.2f, %.2f, %.2f)", inst.scale.x, inst.scale.y, inst.scale.z);
							ImGui::Text("Rotate:     (%.2f, %.2f, %.2f)", inst.rotate.x, inst.rotate.y, inst.rotate.z);
							ImGui::Text("Translate:  (%.2f, %.2f, %.2f)", inst.translate.x, inst.translate.y, inst.translate.z);
							ImGui::Text("Velocity:   (%.2f, %.2f, %.2f)", inst.velocity.x, inst.velocity.y, inst.velocity.z);
							ImGui::Text("Acceleration:(%.2f, %.2f, %.2f)", inst.acceleration.x, inst.acceleration.y, inst.acceleration.z);
							ImGui::Text("Color:      (%.2f, %.2f, %.2f, %.2f)", inst.color.x, inst.color.y, inst.color.z, inst.color.w);
							ImGui::Text("LifeTime:   %.2f", inst.lifeTime);
							ImGui::Text("CurrentTime:%.2f", inst.currentTime);
							ImGui::TreePop();
						}
						++idx;
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();

#endif // USE_IMGUI
}

void ParticleManager::AddSetting(ParticleSetting& setting) {

	// フルパスを作成
	setting.textureFullPath = TextureFolderPath + setting.textureFileName;

	// テクスチャを読み込み
	textureManager->LoadTexture(setting.textureFullPath);

	// 設定を追加
	settings[setting.effectName] = setting;
}

void ParticleManager::AddInstance(const ParticleInstance& instance) {

	// フルパスを取得
	std::string key = instance.setting->textureFullPath;

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

void ParticleManager::UpdateGroups(std::unordered_map<std::string, ParticleGroupNew>& groups) {

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
	if (!std::filesystem::exists(DataFolderPath)) {
		std::filesystem::create_directories(DataFolderPath);
		return; // まだフォルダがない
	}

	for (const auto& entry : std::filesystem::directory_iterator(DataFolderPath)) {
		if (entry.path().extension() == ".json") {
			LoadSettingsFromJSON(entry.path().string());
		}
	}
}

void ParticleManager::ShowEffectList() {

#ifdef USE_IMGUI

	ImGui::Text("Effect List");

	// 全読み込みボタン
	if (ImGui::Button("Reload All Files")) {
		LoadParticleSettingsFromJSON();
	}

	ImGui::Separator();

	// リスト表示
	//ImGui::BeginChild("EffectListScroll", ImVec2(0, 300), true);
	for (auto& [name, setting] : settings) {

		bool isSelected = (currentEditName == name);

		if (ImGui::Selectable(name.c_str(), isSelected)) {
			currentEditName = name;

			// ★重要: 選択した瞬間、本データを一時バッファにコピーする
			tempSetting = setting;
		}
	}
	//ImGui::EndChild();

	ImGui::Separator();

	// 新規作成
	ImGui::InputText("New Name", inputNameBuffer, sizeof(inputNameBuffer));
	if (ImGui::Button("Create New")) {
		std::string newName = inputNameBuffer;
		if (!newName.empty() && settings.find(newName) == settings.end()) {
			// デフォルト設定で作成
			ParticleSetting newSetting;
			newSetting.effectName = newName;
			newSetting.textureFileName = "white.png"; // 仮
			newSetting.textureFullPath = TextureFolderPath + "white.png";
			newSetting.shape = ParticleShape::PLANE;

			settings[newName] = newSetting;
			currentEditName = newName; // 選択状態にする
			tempSetting = newSetting;

			// バッファクリア
			memset(inputNameBuffer, 0, sizeof(inputNameBuffer));
		}
	}

#endif // USE_IMGUI
}

void ParticleManager::ShowParameters() {

#ifdef USE_IMGUI

	if (currentEditName.empty() || settings.find(currentEditName) == settings.end()) {
		ImGui::Text("Select an effect to edit.");
		return;
	}

	// 一時バッファ参照
	ParticleSetting& p = tempSetting;

	// -------------------------------------------------------------
	// 1. 変更チェック
	// -------------------------------------------------------------
	bool isChanged = false;
	json jsonTemp = tempSetting;
	json jsonSaved = settings[currentEditName];
	// 中身が違うなら「変更あり(未Apply)」
	if (jsonTemp != jsonSaved) {
		isChanged = true;
	}

	// -------------------------------------------------------------
	// 2. タイトル表示 (変更があれば * をつける)
	// -------------------------------------------------------------
	if (isChanged) {
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Editing Buffer: %s (*)", p.effectName.c_str());
	}
	else {
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Editing Buffer: %s", p.effectName.c_str());
	}

	ImGui::Separator();

	// -------------------------------------------------------------
	// 3. Applyボタン と Saveボタン
	// -------------------------------------------------------------

	// 変更があるならボタンを「赤色」にする
	if (isChanged) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));        // 赤
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f)); // ホバー時の赤
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));  // 押した時の赤
	}

	if (ImGui::Button("Apply Changes")) {

		// テクスチャのロードもここで行う
		p.textureFullPath = TextureFolderPath + p.textureFileName;
		textureManager->LoadTexture(p.textureFullPath);

		// マップに書き戻す
		settings[currentEditName] = p;
	}

	if (isChanged) ImGui::PopStyleColor(3); // 色を戻す

	ImGui::SameLine();

	if (isChanged) ImGui::BeginDisabled(); // 変更が残っている(未Apply)なら無効化

	if (ImGui::Button("Save to File")) {
		// JSONファイルとして書き出し
		SaveSettingsToJSON(currentEditName);
	}

	// ホバー時にヒントを出すと親切
	if (isChanged && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
		ImGui::SetTooltip("Apply changes first to save."); // 「先にApplyしてね」
	}

	if (isChanged) ImGui::EndDisabled(); // 無効化解除

	// -------------------------------------------------------------
	// 4. Revertボタン
	// -------------------------------------------------------------
	if (ImGui::Button("Revert")) {
		// 元に戻す（本データから再度コピー）
		tempSetting = settings[currentEditName];
	}

	ImGui::Separator();

	// -------------------------------------------------------------
	// 5. テストエミットボタン
	// -------------------------------------------------------------
	if (ImGui::Button("Test Emit (Preview)")) {

		// ★一時的にtempSettingを正式にsettingsに登録する
		std::string tempKey = "TEMP_PREVIEW_" + p.effectName;

		// テクスチャのフルパスを確実に設定
		p.textureFullPath = TextureFolderPath + p.textureFileName;

		// テクスチャを読み込む
		textureManager->LoadTexture(p.textureFullPath);

		// 一時的に設定を登録
		ParticleSetting tempSettingCopy = p;
		tempSettingCopy.effectName = tempKey;
		settings[tempKey] = tempSettingCopy;

		for (int i = 0; i < 10; ++i) {
			ParticleInstance instance;
			instance.setting = &settings[tempKey]; // ★正式に登録された設定への参照

			// 位置
			instance.translate = p.translateRandom ? RandomVector3(p.translateRange) : p.translate;
			// 速度
			instance.velocity = p.velocityRandom ? RandomVector3(p.velocityRange) : p.velocity;
			// 色
			instance.color = p.colorRandom ? RandomVector4(p.colorRange) : p.color;
			// スケール
			instance.scale = p.scaleRandom ? RandomVector3(p.scaleRange) : p.scale;
			// 回転
			instance.rotate = p.rotateRandom ? RandomVector3(p.rotateRange) : p.rotate;
			// 寿命
			instance.lifeTime = p.lifeTimeRandom ? RandomFloat(p.lifeTimeRange.min, p.lifeTimeRange.max) : p.lifeTime;

			AddInstance(instance);
		}

		// ★プレビュー後、一時設定を削除
		settings.erase(tempKey);
	}

	ImGui::Separator();

	// -------------------------------------------------------------
	// 6. タブバーでパラメータ表示
	// -------------------------------------------------------------
	if (ImGui::BeginTabBar("ParamsTab")) {

		// 1. 基本設定タブ
		if (ImGui::BeginTabItem("Basic")) {

			// テクスチャ名の入力
			char texBuff[128];
			strcpy_s(texBuff, p.textureFileName.c_str());

			// 1. 入力欄を表示 (ここでは文字列の更新だけ行う)
			if (ImGui::InputText("Texture", texBuff, sizeof(texBuff))) {
				p.textureFileName = texBuff;
			}

			// 2. ★重要: 「編集が確定した後（Enterやフォーカス外れ）」にロードを実行
			if (ImGui::IsItemDeactivatedAfterEdit()) {

				// ここで初めて重いロード処理を呼ぶ
				textureManager->LoadTexture(p.textureFullPath);
			}

			// フラグ系
			ImGui::Checkbox("Use Billboard", &p.useBillboard);
			//ImGui::Checkbox("Use Gravity", &p.useGravity);

			// 形状 (Combo Box)
			const char* items[] = { "PLANE", "RING", "CYLINDER", "CUBE", "SPHERE", "SHARD" };
			int currentShape = static_cast<int>(p.shape);
			if (ImGui::Combo("Shape", &currentShape, items, IM_ARRAYSIZE(items))) {
				p.shape = static_cast<ParticleShape>(currentShape);
			}

			// 寿命
			ImGui::Separator();
			ImGui::Text("LifeTime");
			ImGui::Checkbox("Random##Life", &p.lifeTimeRandom);
			if (p.lifeTimeRandom) {
				ImGui::DragFloat("Min", &p.lifeTimeRange.min, 0.1f);
				ImGui::DragFloat("Max", &p.lifeTimeRange.max, 0.1f);
			}
			else {
				ImGui::DragFloat("Value", &p.lifeTime, 0.1f);
			}

			ImGui::EndTabItem();
		}

		// 2. Transformタブ
		if (ImGui::BeginTabItem("Transform")) {

			// ヘルパーラムダ: Vector3のMin/Max or Fixedを表示
			auto DrawVec3Control = [](const char* label, bool& isRandom, Vector3Range& range, Vector3& fixed) {
				ImGui::PushID(label);
				ImGui::Text("%s", label);
				ImGui::Checkbox("Random", &isRandom);
				if (isRandom) {
					ImGui::DragFloat3("Min", &range.min.x, 0.01f);
					ImGui::DragFloat3("Max", &range.max.x, 0.01f);
				}
				else {
					ImGui::DragFloat3("Value", &fixed.x, 0.01f);
				}
				ImGui::Separator();
				ImGui::PopID();
				};

			DrawVec3Control("Translate", p.translateRandom, p.translateRange, p.translate);
			DrawVec3Control("Scale", p.scaleRandom, p.scaleRange, p.scale);
			DrawVec3Control("Rotate", p.rotateRandom, p.rotateRange, p.rotate);

			ImGui::EndTabItem();
		}

		// 3. Physics & Colorタブ
		if (ImGui::BeginTabItem("Physics/Color")) {

			// Velocity
			ImGui::Text("Velocity");
			ImGui::Checkbox("Random##Vel", &p.velocityRandom);
			if (p.velocityRandom) {
				ImGui::DragFloat3("Min##Vel", &p.velocityRange.min.x, 0.01f);
				ImGui::DragFloat3("Max##Vel", &p.velocityRange.max.x, 0.01f);
			}
			else {
				ImGui::DragFloat3("Value##Vel", &p.velocity.x, 0.01f);
			}
			ImGui::Separator();

			// Acceleration
			ImGui::DragFloat3("Acceleration", &p.acceleration.x, 0.01f);

			// Color
			ImGui::Text("Color");
			ImGui::Checkbox("Random##Col", &p.colorRandom);
			if (p.colorRandom) {
				ImGui::ColorEdit4("Min##Col", &p.colorRange.min.x);
				ImGui::ColorEdit4("Max##Col", &p.colorRange.max.x);
			}
			else {
				ImGui::ColorEdit4("Value##Col", &p.color.x);
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

#endif // USE_IMGUI
}

void ParticleManager::SaveSettingsToJSON(const std::string& effectName) {

	// 設定が存在するかチェック
	if (settings.find(effectName) == settings.end()) {
		assert(false && "Particle Setting not found for saving");
		return;
	}

	// ディレクトリ確認
	if (!std::filesystem::exists(DataFolderPath)) {
		std::filesystem::create_directories(DataFolderPath);
	}

	// JSONに変換して保存
	json j = settings[effectName];
	std::string path = DataFolderPath + effectName + ".json";

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

	// フルパスの作成
	setting.textureFullPath = TextureFolderPath + setting.textureFileName;

	// テクスチャの読み込み
	textureManager->LoadTexture(setting.textureFullPath);

	// マップに登録
	settings[setting.effectName] = setting;
}

void ParticleManager::CreateGroupResource(ParticleGroupNew& group) {

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

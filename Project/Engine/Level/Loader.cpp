#include "Loader.h"
#include "Model/Model.h"
#include "Model/ModelManager.h"

#include "json.hpp"

#include <fstream>
#include <cassert>
#include <cmath>
#include <numbers>
#include <imgui.h>

void Loader::LoadLevel(const std::string& fileName) {

	/// === 読み込み === ///

	// フルパスを作る
	std::string fullPath = kDefaultDirectory + "/" + fileName;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullPath);

	// ファイルが開けなかった場合
	if (file.fail()) {

		// 実行を止める
		assert(0);
	}

	/// === ファイルチェック === ///

	// ファイル読み込み
	file >> deserialized;

	// 正しいレベルデータファイルの形式かチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルの形式かチェック
	assert(name.compare("scene") == 0);

	/// === オブジェクトの走査 === ///

	// レベルデータ格納用インスタンスを生成
	levelData = std::make_unique<LevelData>();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {

		// オブジェクトの解析
		ParseObject(object);
	}
}

void Loader::PlaceObject() {

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->GetObjects()) {

		// ファイル名からモデルを生成
		std::unique_ptr<Model> model = std::make_unique<Model>();
		// モデルの読み込み
		ModelManager::GetInstance()->LoadModelData(kDefaultDirectory, objectData.fileName + ".obj");
		// モデルの初期化
		model->Initialize(kDefaultDirectory, objectData.fileName + ".obj");

		// 3Dオブジェクトの生成
		std::unique_ptr<Object3d> object = std::make_unique<Object3d>();
		object->Initialize();
		// モデルの設定
		object->SetModel(model.get());

		// トランスフォームの設定
		object->SetScale(objectData.scale);			  // スケール
		object->SetRotate(objectData.rotation);		  // 回転
		object->SetTranslate(objectData.translation); // 位置

		// リストに登録
		models.emplace(objectData.fileName, std::move(model));
		objects.emplace(objectData.fileName, std::move(object));
	}
}

void Loader::PlacePlayer() {

	int index = 0;

	for (auto& playerData : levelData->GetPlayers()) {

		// モデルを生成
		std::unique_ptr<Model> model = std::make_unique<Model>();
		// モデルの読み込み
		ModelManager::GetInstance()->LoadModelData("Resources/Player", "player.obj");
		// モデルの初期化
		model->Initialize("Resources/Player", "player.obj");

		// 3Dオブジェクトの生成
		std::unique_ptr<Object3d> object = std::make_unique<Object3d>();
		object->Initialize();

		// モデルの設定
		object->SetModel(model.get());

		// トランスフォームの設定
		object->SetRotate(playerData.rotation); // 回転
		object->SetTranslate(playerData.translation); // 位置

		// インデックスを設定
		playerData.index = ++index;

		// リストに登録
		objects.emplace("Player" + std::to_string(playerData.index), std::move(object));
		models.emplace("Player" + std::to_string(playerData.index), std::move(model));
	}
}

void Loader::PlaceEnemy() {

	int index = 0;

	for (auto& enemyData : levelData->GetEnemies()) {

		// モデルを生成
		std::unique_ptr<Model> model = std::make_unique<Model>();
		// モデルの読み込み
		ModelManager::GetInstance()->LoadModelData("Resources/Enemy", "enemy.obj");
		// モデルの初期化
		model->Initialize("Resources/Enemy", "enemy.obj");

		// 3Dオブジェクトの生成
		std::unique_ptr<Object3d> object = std::make_unique<Object3d>();
		object->Initialize();

		// モデルの設定
		object->SetModel(model.get());

		// トランスフォームの設定
		object->SetRotate(enemyData.rotation); // 回転
		object->SetTranslate(enemyData.translation); // 位置

		// インデックスを設定
		enemyData.index = ++index;

		// リストに登録
		models.emplace("Enemy" + std::to_string(enemyData.index), std::move(model));
		objects.emplace("Enemy" + std::to_string(enemyData.index), std::move(object));
	}
}

void Loader::Update() {

	// オブジェクトの更新
	for (const auto& object : objects) {
		object.second->Update();
	}
}

void Loader::Draw() {

	// オブジェクトの描画
	for (const auto& object : objects) {
		object.second->Draw();
	}
}

void Loader::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("Level");

	// オブジェクトのImGui表示
	for (const auto& object : objects) {

		if(ImGui::TreeNode(object.first.c_str())) {
			// オブジェクトのImGui表示
			object.second->ShowImGui();
			models[object.first]->ShowImGui();
			ImGui::TreePop();
		}
	}

	ImGui::End();

#endif // USE_IMGUI
}

void Loader::ParseObject(nlohmann::json& object) {

	// オブジェクトが正しい形式かチェック
	assert(object.contains("type"));

	if(object.contains("disabled")){

		// 有効無効フラグ
		bool disabled = object["disabled"].get<bool>();

		// 無効な場合は何もしない
		if (disabled) {
			
			// 配置しない(スキップする)
			return;
		}
	}

	// 種別を取得
	std::string type = object["type"].get<std::string>();

	/// === メッシュの読み込み === ///

	// MESH
	if (type.compare("MESH") == 0) {

		// 要素追加
		levelData->GetObjects().emplace_back(LevelData::ObjectData{});
		// 今追加した要素の参照を得る
		LevelData::ObjectData& objectData = levelData->GetObjects().back();

		if (object.contains("name")) {
			// ファイル名を取得
			objectData.fileName = object["name"];
		}

		// トランスフォームのパラメータ読み込み
		nlohmann::json& transform = object["transform"];

		// 平行移動
		objectData.translation.x = transform["translation"][0];
		objectData.translation.y = transform["translation"][2];
		objectData.translation.z = transform["translation"][1];

		// 回転角
		objectData.rotation.x = transform["rotation"][0] * (std::numbers::pi_v<float> / 180.0f);
		objectData.rotation.y = transform["rotation"][2] * (std::numbers::pi_v<float> / 180.0f);
		objectData.rotation.z = transform["rotation"][1] * (std::numbers::pi_v<float> / 180.0f);

		// スケーリング
		objectData.scale.x = transform["scaling"][0];
		objectData.scale.y = transform["scaling"][2];
		objectData.scale.z = transform["scaling"][1];

		// モデルの読み込み
		ModelManager::GetInstance()->LoadModelData(kDefaultDirectory, objectData.fileName + ".obj");

		//TODO:コライダーの読み込み
	}
	else if (type.compare("PlayerSpawn") == 0) {

		// 要素追加
		levelData->GetPlayers().emplace_back(LevelData::PlayerSpawnData{});
		// 今追加した要素の参照を得る
		LevelData::PlayerSpawnData& playerData = levelData->GetPlayers().back();

		// トランスフォームのパラメータ読み込み
		nlohmann::json& transform = object["transform"];

		// 平行移動
		playerData.translation.x = transform["translation"][0];
		playerData.translation.y = transform["translation"][2];
		playerData.translation.z = transform["translation"][1];

		// 回転角
		playerData.rotation.x = transform["rotation"][0] * (std::numbers::pi_v<float> / 180.0f);
		playerData.rotation.y = transform["rotation"][2] * (std::numbers::pi_v<float> / 180.0f);
		playerData.rotation.z = transform["rotation"][1] * (std::numbers::pi_v<float> / 180.0f);
	}
	else if (type.compare("EnemySpawn") == 0) {

		// 要素追加
		levelData->GetEnemies().emplace_back(LevelData::EnemySpawnData{});
		// 今追加した要素の参照を得る
		LevelData::EnemySpawnData& enemyData = levelData->GetEnemies().back();

		// トランスフォームのパラメータ読み込み
		nlohmann::json& transform = object["transform"];

		// 平行移動
		enemyData.translation.x = transform["translation"][0];
		enemyData.translation.y = transform["translation"][2];
		enemyData.translation.z = transform["translation"][1];

		// 回転角
		enemyData.rotation.x = transform["rotation"][0] * (std::numbers::pi_v<float> / 180.0f);
		enemyData.rotation.y = transform["rotation"][2] * (std::numbers::pi_v<float> / 180.0f);
		enemyData.rotation.z = transform["rotation"][1] * (std::numbers::pi_v<float> / 180.0f);
	}

	/// === ツリー構造の走査 === ///

	// 子ノードがある場合
	if (object.contains("children")) {

		// 再帰的に呼び出して解析する
		for (nlohmann::json& child : object["children"]) {
			// 子ノードの解析
			ParseObject(child);
		}
	}
}

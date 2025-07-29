#include "Loader.h"
#include "3D/Model.h"
#include "3D/ModelManager.h"
#include "json.hpp"

#include <fstream>
#include <cassert>
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
		objectData.rotation.x = transform["rotation"][0];
		objectData.rotation.y = transform["rotation"][2];
		objectData.rotation.z = transform["rotation"][1];

		// スケーリング
		objectData.scale.x = transform["scaling"][0];
		objectData.scale.y = transform["scaling"][2];
		objectData.scale.z = transform["scaling"][1];

		// モデルの読み込み
		ModelManager::GetInstance()->LoadModelData(kDefaultDirectory, objectData.fileName + ".obj");

		//TODO:コライダーの読み込み
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

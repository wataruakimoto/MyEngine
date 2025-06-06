#include "Loader.h"
#include "json.hpp"
#include "LevelData.h"

#include <fstream>
#include <cassert>

void Loader::LoadLevel(const std::string& filePath) {

	/// === 読み込み === ///

	// フルパスを作る
	std::string fullPath = kDefaultDirectory + filePath;

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

	// JSON文字列から読み込んだデータ
	nlohmann::json deserialized;

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
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {

		// オブジェクトが正しい形式かチェック
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		/// === メッシュの読み込み === ///

		// MESH
		if (type.compare("MESH") == 0) {

			// 要素追加
			levelData->GetObjects().emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->GetObjects().back();

			if (object.contains("file_name")) {
				// ファイル名を取得
				objectData.fileName = object["file_name"];
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
		}

		/// === ツリー構造の走査 === ///

		// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を操作する
		if (object.contains("children")) {
			
		}
	}
}

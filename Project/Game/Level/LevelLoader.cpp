#include "LevelLoader.h"

#include <fstream>
#include <cassert>

using namespace Engine;

void LevelLoader::LoadLevel(const std::string& filePath) {

	// フルパスを作成
	std::string fullPath = kDataDirectoryPath_ + "/" + filePath;
	
	// ファイルを開く
	std::ifstream file(fullPath);
	
	assert(file.is_open());

	nlohmann::json jsonData;

	file >> jsonData;

	// レベルデータの初期化
	levelData_ = {};

	// プレイヤーの開始位置の読み込み
	if (jsonData.contains("player") && jsonData["player"].is_object()) {

		const auto& playerData = jsonData["player"];

		if (playerData.contains("position")) {
			levelData_.playerPosition = playerData["position"].get<Vector3>();
		}
	}

	// 敵の出現データの読み込み
	if (jsonData.contains("enemies") && jsonData["enemies"].is_array()) {
		for (const auto& enemyData : jsonData["enemies"]) {
			levelData_.enemySpawnDatas.push_back(enemyData.get<EnemySpawnData>());
		}
	}
}

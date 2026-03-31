#pragma once

#include "Vector3.h"

#include <string>
#include <vector>

// 敵の種類
enum class EnemyType {
	Normal,     // 通常（弾を撃つ）
	Kamikaze,   // 特攻（プレイヤーに突っ込む）
};

// 敵の出現データ
struct EnemySpawnData {
	Engine::Vector3 position; // 出現位置
	EnemyType type; // 敵の種類
};

// 障害物の出現データ
struct ObstacleSpawnData {
	Engine::Vector3 scale; // 大きさ
	Engine::Vector3 position; // 出現位置
};

// ゲームのデータ
struct GameLevelData {
	Engine::Vector3 playerPosition; // プレイヤーの開始位置
	std::vector<EnemySpawnData> enemySpawnDatas; // 敵の出現データのリスト
	std::vector<ObstacleSpawnData> obstacleSpawnDatas; // 障害物の出現データのリスト
};

inline EnemyType EnemyTypeFromString(const std::string& typeStr) {

	if (typeStr == "Normal") { return EnemyType::Normal; }
	if (typeStr == "Kamikaze") { return EnemyType::Kamikaze; }
	// 必要に応じて追加

	// 不明値はとりあえずNormal or 例外/アサートなど方針を決める
	return EnemyType::Normal;
}

inline void from_json(const nlohmann::json& j, EnemySpawnData& data) {
	j.at("position").get_to(data.position);
	
	std::string typeStr;
	j.at("type").get_to(typeStr);
	data.type = EnemyTypeFromString(typeStr);
}

inline void from_json(const nlohmann::json& j, ObstacleSpawnData& data) {
	j.at("scale").get_to(data.scale);
	j.at("position").get_to(data.position);
}

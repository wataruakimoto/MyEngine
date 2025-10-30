#pragma once

#include "Vector3.h"

#include "json.hpp"

#include <list>

/// === レベルデータ格納 === ///
class LevelData {

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	// オブジェクトデータ
	struct ObjectData {
		std::string fileName; // ファイル名
		nlohmann::json objects; // オブジェクトのデータ
		Vector3 translation; // 位置
		Vector3 rotation; // 回転
		Vector3 scale; // スケール
	};

	// 自キャラの生成データ
	struct PlayerSpawnData {
		Vector3 translation; // 位置
		Vector3 rotation; // 回転
		int index; // インデックス
	};

	// 敵キャラの生成データ
	struct EnemySpawnData {
		Vector3 translation; // 位置
		Vector3 rotation; // 回転
		int index; // インデックス
	};

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// オブジェクトデータのリストの取得
	/// </summary>
	/// <returns></returns>
	std::list<ObjectData>& GetObjects() { return objects; }

	/// <summary>
	/// 自キャラのリストの取得
	/// </summary>
	/// <returns></returns>
	std::list<PlayerSpawnData>& GetPlayers() { return players; }

	/// <summary>
	/// 敵キャラのリストの取得
	/// </summary>
	/// <returns></returns>
	std::list<EnemySpawnData>& GetEnemies() { return enemies; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// オブジェクトデータのリスト
	std::list<ObjectData> objects;

	// 自キャラのリスト
	std::list<PlayerSpawnData> players;

	// 敵キャラのリスト
	std::list<EnemySpawnData> enemies;
};


#pragma once
#include "json.hpp"
#include "Math/Vector3.h"
#include <list>

/// === レベルデータ格納 === ///
class LevelData {

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	struct ObjectData {
		std::string fileName; // ファイル名
		Vector3 translation; // 位置
		Vector3 rotation; // 回転
		Vector3 scale; // スケール
	};

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// オブジェクトリストの取得
	/// </summary>
	/// <returns></returns>
	std::list<nlohmann::json>& GetObjects() { return objects; }


///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// オブジェクトのリスト
	std::list<nlohmann::json> objects;
};


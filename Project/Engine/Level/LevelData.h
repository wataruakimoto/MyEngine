#pragma once
#include "Math/Vector3.h"
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

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// オブジェクトデータのリストの取得
	/// </summary>
	/// <returns></returns>
	std::list<ObjectData>& GetObjects() { return objects; }


///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// オブジェクトデータのリスト
	std::list<ObjectData> objects;
};


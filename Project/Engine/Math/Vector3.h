#pragma once

#include <json.hpp>

namespace Engine {

	/// <summary>
	/// 3次元ベクトル
	/// </summary>
	struct Vector3 {
		float x, y, z;
	};

	// JSON用のシリアライズ・デシリアライズ定義
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector3, x, y, z)
}
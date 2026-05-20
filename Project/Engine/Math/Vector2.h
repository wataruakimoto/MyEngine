#pragma once

#include <json.hpp>

namespace Engine {

	/// <summary>
	/// 2次元ベクトル
	/// </summary>
	struct Vector2 {
		float x, y;
	};

	// JSON用のシリアライズ・デシリアライズ定義
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2, x, y)
}
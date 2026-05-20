#pragma once

#include <json.hpp>

namespace Engine {

	/// <summary>
	/// 4次元ベクトル
	/// </summary>
	struct Vector4 {
		float x, y, z, w;
	};

	// JSON用のシリアライズ・デシリアライズ定義
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector4, x, y, z, w)
}
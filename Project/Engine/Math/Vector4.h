#pragma once

#include <json.hpp>

namespace Engine {

	struct Vector4 {
		float x;
		float y;
		float z;
		float w;
	};

	// JSON用のシリアライズ・デシリアライズ定義
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector4, x, y, z, w)
}
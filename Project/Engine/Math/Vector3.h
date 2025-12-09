#pragma once

#include <json.hpp>

struct Vector3 {
	float x;
	float y;
	float z;
};

// JSON用のシリアライズ・デシリアライズ定義
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector3, x, y, z)
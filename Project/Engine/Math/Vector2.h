#pragma once

#include <json.hpp>

struct Vector2 {
	float x;
	float y;
};

// JSON用のシリアライズ・デシリアライズ定義
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2, x, y)
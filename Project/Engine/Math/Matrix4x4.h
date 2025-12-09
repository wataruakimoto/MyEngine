#pragma once

#include <json.hpp>

struct Matrix4x4 {
	float m[4][4];
};

/// JSON用のシリアライズ・デシリアライズ定義
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Matrix4x4, m)
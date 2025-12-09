#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include <json.hpp>

// floatの範囲
struct floatRange {
	float min;
	float max;
};

// 2次元の範囲
struct Vector2Range {
	Vector2 min;
	Vector2 max;
};

// 3次元の範囲
struct Vector3Range {
	Vector3 min;
	Vector3 max;
};

// 4次元の範囲
struct Vector4Range {
	Vector4 min;
	Vector4 max;
};

namespace MathRandom {

	// 指定された範囲でランダムなfloatを生成
	float RandomFloat(float min, float max);

	// 指定された範囲でランダムなfloatを生成
	float RandomFloat(const floatRange& range);

	// 指定された範囲でランダムなVector2を生成
	Vector2 RandomVector2(const Vector2Range& range);

	// 指定された範囲でランダムなVector3を生成
	Vector3 RandomVector3(const Vector3Range& range);

	// 指定された範囲でランダムなVector4を生成
	Vector4 RandomVector4(const Vector4Range& range);
}

// JSON用のシリアライズ・デシリアライズ定義
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(floatRange, min, max)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2Range, min, max)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector3Range, min, max)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector4Range, min, max)
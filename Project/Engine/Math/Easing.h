#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Easing {

	float Lerp(float start, float end, float t);

	Vector2 Lerp(const Vector2& start, const Vector2& end, float t);

	Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

	Vector4 Lerp(const Vector4& start, const Vector4& end, float t);

	float EaseInOut(float t);

	float EaseInCubic(float t);

	float EaseOutCubic(float t);

	float EaseOutQuart(float t);

	float EaseOutBounce(float t);
}

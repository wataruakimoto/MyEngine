#include "Easing.h"
#include "MathVector.h"

#include <cmath>

using namespace MathVector;

float Easing::Lerp(float start, float end, float t) {

	return start + (end - start) * t;
}

Vector2 Easing::Lerp(const Vector2& start, const Vector2& end, float t) {
	
	return start + (end - start) * t;
}

Vector3 Easing::Lerp(const Vector3& start, const Vector3& end, float t) {
	
	return start + (end - start) * t;
}

Vector4 Easing::Lerp(const Vector4& start, const Vector4& end, float t) {
	
	return start + (end - start) * t;
}

float Easing::EaseInOut(float t) {
	
	if (t < 0.5f) {

		return 2.0f * t * t;
	}
	else {

		return -1.0f + (4.0f - 2.0f * t) * t;
	}
}

float Easing::EaseInCubic(float t) {
	
	return t * t * t;
}

float Easing::EaseOutCubic(float t) {
	
	return 1.0f - std::powf(1.0f - t, 3);
}

float Easing::EaseOutQuart(float t) {
	
	return 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t) * (1.0f - t);
}

float Easing::EaseOutBounce(float t) {

	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1.0f / d1) {

		return n1 * t * t;
	}
	else if (t < 2.0f / d1) {

		return n1 * (t -= 1.5f / d1) * t + 0.75f;
	}
	else if (t < 2.5f / d1) {

		return n1 * (t -= 2.25f / d1) * t + 0.9375f;
	}
	else {

		return n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}
}

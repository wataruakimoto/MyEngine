#include "MathVector.h"
#include "WinApp.h"

#define _USE_MATH_DEFINES

#include <cmath>
#include <cassert>
#include <numbers>

using namespace Engine;

Vector2 MathVector::Add(const Vector2& v1, const Vector2& v2) {

	Vector2 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;

	return result;
}

Vector3 MathVector::Add(const Vector3& v1, const Vector3& v2) {

	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

Vector4 MathVector::Add(const Vector4& v1, const Vector4& v2) {
	
	Vector4 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	result.w = v1.w + v2.w;

	return result;
}

Vector2 MathVector::Subtract(const Vector2& v1, const Vector2& v2) {
	
	Vector2 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;

	return result;
}

Vector3 MathVector::Subtract(const Vector3& v1, const Vector3& v2) {

	Vector3 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}

Vector4 MathVector::Subtract(const Vector4& v1, const Vector4& v2) {
	
	Vector4 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	result.w = v1.w - v2.w;

	return result;
}

Vector2 MathVector::Multiply(float s, const Vector2& v) {
	
	Vector2 result;

	result.x = v.x * s;
	result.y = v.y * s;

	return result;
}

Vector3 MathVector::Multiply(float s, const Vector3& v) {

	Vector3 result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;

	return result;
}

Vector4 MathVector::Multiply(float s, const Vector4& v) {
	
	Vector4 result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	result.w = v.w * s;

	return result;
}

float MathVector::Length(float f1, float f2) {

	return sqrtf(f1 * f1 + f2 * f2);
}

float MathVector::Length(const Vector2& v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

float MathVector::Length(const Vector3& v) {

	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float MathVector::Length(const Vector4& v) {
	
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

float MathVector::Distance(const Vector2& v1, const Vector2& v2) {
	
	return Length(v2 - v1);
}

float MathVector::Distance(const Vector3& v1, const Vector3& v2) {
	
	return Length(v2 - v1);
}

float MathVector::Distance(const Vector4& v1, const Vector4& v2) {
	
	return Length(v2 - v1);
}

Vector3 MathVector::Normalize(const Vector3& v) {

	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	if (length == 0) { return Vector3(0.0f, 0.0f, 0.0f); }

	return Vector3(v.x / length, v.y / length, v.z / length);
}

Vector3 MathVector::Transform(const Vector3& vector, const Matrix4x4& matrix) {

	// w = 1 がデカルト座標系であるので(x,y,z,1)のベクトルとしてmatrixとの積をとる
	Vector3 resultTransform = {};

	resultTransform.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	resultTransform.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	resultTransform.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	// ベクトルに対して基本的な操作を行う行列でwが0になることはありえない
	assert(w != 0.0f);

	// w = 1 がデカルト座標系であるので、w除算することで同次座標をデカルト座標に戻す
	resultTransform.x /= w;
	resultTransform.y /= w;
	resultTransform.z /= w;

	return resultTransform;
}

Vector3 MathVector::TransformNormal(const Vector3& v, const Matrix4x4& m) {

	Vector3 resultTransformNormal;

	resultTransformNormal.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	resultTransformNormal.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
	resultTransformNormal.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];

	return resultTransformNormal;
}

Vector2 MathVector::ConvertWorldToScreen(const Vector3& worldPos, const Matrix4x4& viewProjectMatrix) {

	// ワールド座標をクリップ座標に変換
	Vector3 clipPos = Transform(worldPos, viewProjectMatrix);

	// クリップ座標をNDC座標に変換
	Vector2 ndc = { clipPos.x / clipPos.z, clipPos.y / clipPos.z };

	// NDC座標をスクリーン座標に変換
	Vector2 screenPos = {
		(ndc.x + 1.0f) / 2.0f * float(WinApp::kClientWidth),
		(-ndc.y + 1.0f) / 2.0f * float(WinApp::kClientHeight) // y軸は上下反転
	};

	return screenPos;
}

Vector3 MathVector::ConvertScreenToWorld(const Vector2& screenPos, const Matrix4x4& inverseViewProjectionMatrix, float distance) {
	
	// NDC座標に変換
	Vector2 ndc = { 
		(screenPos.x / float(WinApp::kClientWidth)) * 2.0f - 1.0f,
		1.0f - (screenPos.y / float(WinApp::kClientHeight)) * 2.0f // y軸は上下反転
	};

	// 近平面と遠平面のクリップ座標
	Vector3 nearClip = { ndc.x, ndc.y, 0.0f }; // 近平面
	Vector3 farClip = { ndc.x, ndc.y, 1.0f };  // 遠平面

	// クリップ座標をワールド座標に変換
	Vector3 nearWorld = Transform(nearClip, inverseViewProjectionMatrix);
	Vector3 farWorld = Transform(farClip, inverseViewProjectionMatrix);

	// 方向ベクトルの計算
	Vector3 direction = Normalize(farWorld - nearWorld);

	// 座標の計算
	Vector3 resultPosition = nearWorld + (direction * distance);

	return resultPosition;
}

float MathVector::ConvertDegreesToRadians(float degrees) {
	
	return degrees * (std::numbers::pi_v<float> / 180.0f);
}

float MathVector::ConvertRadiansToDegrees(float radians) {
	
	return radians * (180.0f / std::numbers::pi_v<float>);
}

Vector2 MathVector::operator+(const Vector2& v1, const Vector2& v2) {
	
	return Add(v1, v2);
}

Vector2 MathVector::operator+(const Vector2& v) {
	
	return v;
}

Vector2& MathVector::operator+=(Vector2& v1, const Vector2& v2) {

	v1.x += v2.x;
	v1.y += v2.y;

	return v1;
}

Vector3 MathVector::operator+(const Vector3& v1, const Vector3& v2) {

	return Add(v1, v2);
}

Vector3 MathVector::operator+(const Vector3& v) {

	return v;
}

Vector3& MathVector::operator+=(Vector3& v1, const Vector3& v2) {

	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;

	return v1;
}

Vector4 MathVector::operator+(const Vector4& v1, const Vector4& v2) {
	
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

Vector4 MathVector::operator+(const Vector4& v) {
	
	return v;
}

Vector4& MathVector::operator+=(Vector4& v1, const Vector4& v2) {
	
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	v1.w += v2.w;

	return v1;
}

Vector2 MathVector::operator-(const Vector2& v1, const Vector2& v2) {
	
	return Subtract(v1, v2);
}

Vector2 MathVector::operator-(const Vector2& v) {
	
	return { -v.x, -v.y };
}

Vector2& MathVector::operator-=(Vector2& v1, const Vector2& v2) {
	
	v1.x -= v2.x;
	v1.y -= v2.y;

	return v1;
}

Vector3 MathVector::operator-(const Vector3& v1, const Vector3& v2) {

	return Subtract(v1, v2);
}

Vector3 MathVector::operator-(const Vector3& v) {

	return { -v.x, -v.y, -v.z };
}

Vector3& MathVector::operator-=(Vector3& v1, const Vector3& v2) {

	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}

Vector4 MathVector::operator-(const Vector4& v1, const Vector4& v2) {
	
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

Vector4 MathVector::operator-(const Vector4& v) {
	
	return { -v.x, -v.y, -v.z, -v.w };
}

Vector4& MathVector::operator-=(Vector4& v1, const Vector4& v2) {
	
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	v1.w -= v2.w;

	return v1;
}

Vector2 MathVector::operator*(float s, const Vector2& v) {
	
	return { v.x * s, v.y * s };
}

Vector2 MathVector::operator*(const Vector2& v, float s) {
	
	return s * v;
}

Vector2& MathVector::operator*=(Vector2& v, float s) {
	
	v.x *= s;
	v.y *= s;

	return v;
}

Vector3 MathVector::operator*(float s, const Vector3& v) {

	return Multiply(s, v);
}

Vector3 MathVector::operator*(const Vector3& v, float s) {

	return s * v;
}

Vector3& MathVector::operator *= (Vector3 & v, float s) {

	v.x *= s;
	v.y *= s;
	v.z *= s;

	return v;
}

Vector4 MathVector::operator*(float s, const Vector4& v) {
	
	return { v.x * s, v.y * s, v.z * s, v.w * s };
}

Vector4 MathVector::operator*(const Vector4& v, float s) {
	
	return s * v;
}

Vector4& MathVector::operator*=(Vector4& v, float s) {
	
	v.x *= s;
	v.y *= s;
	v.z *= s;
	v.w *= s;

	return v;
}

Vector2 MathVector::operator/(const Vector2& v, float s) {
	
	return Multiply(1.0f / s, v);
}

Vector2& MathVector::operator/=(Vector2& v, float s) {
	
	v.x /= s;
	v.y /= s;

	return v;
}

Vector3 MathVector::operator/(const Vector3& v, float s) {

	return Multiply(1.0f / s, v);
}

Vector3& MathVector::operator/=(Vector3& v, float s) {

	v.x /= s;
	v.y /= s;
	v.z /= s;

	return v;
}

Vector4 MathVector::operator/(const Vector4& v, float s) {
	
	return Multiply(1.0f / s, v);
}

Vector4& MathVector::operator/=(Vector4& v, float s) {
	
	v.x /= s;
	v.y /= s;
	v.z /= s;
	v.w /= s;

	return v;
}

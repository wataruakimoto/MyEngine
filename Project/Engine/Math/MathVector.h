#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

namespace MathVector {

///-------------------------------------------/// 
/// 静的メンバ関数
///-------------------------------------------///

	// 加算
	Vector2 Add(const Vector2& v1, const Vector2& v2);
	Vector3 Add(const Vector3& v1, const Vector3& v2);

	// 減算
	Vector2 Subtract(const Vector2& v1, const Vector2& v2);
	Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	// 積
	Vector3 Multiply(float scalar, const Vector3& v);

	// 長さ
	float Length(float f1, float f2);
	float Length(const Vector2& v);
	float Length(const Vector3& v);

	// 距離
	float Distance(const Vector2& v1, const Vector2& v2);
	float Distance(const Vector3& v1, const Vector3& v2);

	// 正規化
	Vector3 Normalize(const Vector3& v);

	// 座標変換
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	// ベクトル変換
	Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	// ワールド座標→スクリーン座標
	Vector2 ConvertWorldToScreen(const Vector3& worldPos, const Matrix4x4& viewProjectionMatrix);

	// スクリーン座標→ワールド座標
	Vector3 ConvertScreenToWorld(const Vector2& screenPos, const Matrix4x4& inverseViewProjectionMatrix, float nearZ, float farZ);

///-------------------------------------------/// 
/// オペレーター演算子
///-------------------------------------------///

	Vector2 operator+(const Vector2& v1, const Vector2& v2);
	Vector2 operator+(const Vector2& v);
	Vector2& operator+=(Vector2& v1, const Vector2& v2);

	Vector3 operator+(const Vector3& v1, const Vector3& v2);
	Vector3 operator+(const Vector3& v);
	Vector3& operator+=(Vector3& v1, const Vector3& v2);

	Vector2 operator-(const Vector2& v1, const Vector2& v2);
	Vector2 operator-=(Vector2& v1, const Vector2& v2);

	Vector3 operator-(const Vector3& v1, const Vector3& v2);
	Vector3 operator-(const Vector3& v);
	Vector3& operator-=(Vector3& v1, const Vector3& v2);


	Vector3 operator*(float s, const Vector3& v);
	Vector3 operator*(const Vector3& v, float s);
	Vector3& operator*=(Vector3& v, float s);

	Vector3 operator/(const Vector3& v, float s);
	Vector3& operator/=(Vector3& v, float s);
}
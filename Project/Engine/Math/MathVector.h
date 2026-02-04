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
	Vector4 Add(const Vector4& v1, const Vector4& v2);

	// 減算
	Vector2 Subtract(const Vector2& v1, const Vector2& v2);
	Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	Vector4 Subtract(const Vector4& v1, const Vector4& v2);

	// 積
	Vector2 Multiply(float s, const Vector2& v);
	Vector3 Multiply(float s, const Vector3& v);
	Vector4 Multiply(float s, const Vector4& v);

	// 長さ
	float Length(float f1, float f2);
	float Length(const Vector2& v);
	float Length(const Vector3& v);
	float Length(const Vector4& v);

	// 距離
	float Distance(const Vector2& v1, const Vector2& v2);
	float Distance(const Vector3& v1, const Vector3& v2);
	float Distance(const Vector4& v1, const Vector4& v2);

	// 正規化
	Vector3 Normalize(const Vector3& v);

	// 座標変換
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	// ベクトル変換
	Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	// ワールド座標→スクリーン座標
	Vector2 ConvertWorldToScreen(const Vector3& worldPos, const Matrix4x4& viewProjectionMatrix);

	// スクリーン座標→ワールド座標
	Vector3 ConvertScreenToWorld(const Vector2& screenPos, const Matrix4x4& inverseViewProjectionMatrix, float distance);

	// 角度をラジアンに変換
	float ConvertDegreesToRadians(float degrees);

	// ラジアンを角度に変換
	float ConvertRadiansToDegrees(float radians);

///-------------------------------------------/// 
/// オペレーター演算子
///-------------------------------------------///

	/// ===== 加算 ===== ///

	Vector2 operator+(const Vector2& v1, const Vector2& v2);
	Vector2 operator+(const Vector2& v);
	Vector2& operator+=(Vector2& v1, const Vector2& v2);

	Vector3 operator+(const Vector3& v1, const Vector3& v2);
	Vector3 operator+(const Vector3& v);
	Vector3& operator+=(Vector3& v1, const Vector3& v2);

	Vector4 operator+(const Vector4& v1, const Vector4& v2);
	Vector4 operator+(const Vector4& v);
	Vector4& operator+=(Vector4& v1, const Vector4& v2);

	/// ===== 減算 ===== ///

	Vector2 operator-(const Vector2& v1, const Vector2& v2);
	Vector2 operator-(const Vector2& v);
	Vector2& operator-=(Vector2& v1, const Vector2& v2);

	Vector3 operator-(const Vector3& v1, const Vector3& v2);
	Vector3 operator-(const Vector3& v);
	Vector3& operator-=(Vector3& v1, const Vector3& v2);

	Vector4 operator-(const Vector4& v1, const Vector4& v2);
	Vector4 operator-(const Vector4& v);
	Vector4& operator-=(Vector4& v1, const Vector4& v2);

	/// ===== 乗算 ===== ///

	Vector2 operator*(float s, const Vector2& v);
	Vector2 operator*(const Vector2& v, float s);
	Vector2& operator*=(Vector2& v, float s);

	Vector3 operator*(float s, const Vector3& v);
	Vector3 operator*(const Vector3& v, float s);
	Vector3& operator*=(Vector3& v, float s);

	Vector4 operator*(float s, const Vector4& v);
	Vector4 operator*(const Vector4& v, float s);
	Vector4& operator*=(Vector4& v, float s);

	/// ===== 除算 ===== ///

	Vector2 operator/(const Vector2& v, float s);
	Vector2& operator/=(Vector2& v, float s);

	Vector3 operator/(const Vector3& v, float s);
	Vector3& operator/=(Vector3& v, float s);

	Vector4 operator/(const Vector4& v, float s);
	Vector4& operator/=(Vector4& v, float s);
}
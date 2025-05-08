#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

// 加算
Vector2 Add(const Vector2& v1, const Vector2& v2);

Vector2& operator+=(Vector2& v1, const Vector2& v2);

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator+(const Vector3& v);
Vector3& operator+=(Vector3& v1, const Vector3& v2);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v);
Vector3& operator-=(Vector3& v1, const Vector3& v2);

// 積
Vector3 Multiply(float scalar, const Vector3& v);

Vector3 operator*(float s, const Vector3& v);
Vector3 operator*(const Vector3& v, float s);
Vector3& operator*=(Vector3& v, float s);

Vector3 operator/(const Vector3& v, float s);
Vector3& operator/=(Vector3& v, float s);

// 正規化
Vector3 Normalize(const Vector3& v);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
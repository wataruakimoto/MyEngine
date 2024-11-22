#include "MathVector.h"
#define _USE_MATH_DEFINES
#include <cmath>

Vector2 Add(const Vector2& v1, const Vector2& v2) {

	Vector2 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;

	return result;
}

Vector2& operator+=(Vector2& v1, const Vector2& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;

	return v1;
}

Vector3 Add(const Vector3& v1, const Vector3& v2) {

	Vector3 resultAdd;

	resultAdd.x = v1.x + v2.x;
	resultAdd.y = v1.y + v2.y;
	resultAdd.z = v1.z + v2.z;

	return resultAdd;
}

Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	return Add(v1, v2);
}

Vector3 operator+(const Vector3& v)
{
	return v;
}

Vector3& operator+=(Vector3& v1, const Vector3& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;

	return v1;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {

		Vector3 resultSubtract;

		resultSubtract.x = v1.x - v2.x;
		resultSubtract.y = v1.y - v2.y;
		resultSubtract.z = v1.z - v2.z;

		return resultSubtract;
	}

Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	return Subtract(v1, v2);
}

Vector3 operator-(const Vector3& v)
{ 
	return { -v.x, -v.y, -v.z };
}

Vector3& operator-=(Vector3& v1, const Vector3& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}

Vector3 Multiply(float scalar, const Vector3& v) {

	    Vector3 resultMultiply;

	    resultMultiply.x = v.x * scalar;
	    resultMultiply.y = v.y * scalar;
	    resultMultiply.z = v.z * scalar;

	    return resultMultiply;
    }

Vector3 operator*(float s, const Vector3& v)
{
	return Multiply(s, v);
}

Vector3 operator*(const Vector3& v, float s)
{
	return s * v;
}

Vector3& operator*=(Vector3& v, float s)
{
	v.x *= s;
	v.y *= s;
	v.z *= s;

	return v;
}

Vector3 operator/(const Vector3& v, float s)
{
	return Multiply(1.0f / s, v);
}

Vector3& operator/=(Vector3& v, float s)
{
	v.x /= s;
	v.y /= s;
	v.z /= s;

	return v;
}

Vector3 Normalize(const Vector3& v) {

	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	if (length == 0) { return Vector3(0.0f, 0.0f, 0.0f);}
    
	return Vector3(v.x / length, v.y / length, v.z / length);
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {

	Vector3 resultTransformNormal;

	resultTransformNormal.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	resultTransformNormal.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
	resultTransformNormal.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];

	return resultTransformNormal;
}
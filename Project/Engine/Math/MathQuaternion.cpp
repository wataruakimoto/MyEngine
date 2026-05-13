#include "MathQuaternion.h"
#include "MathVector.h"
#include "Easing.h"

#include <cmath>

using namespace Engine;
using namespace Easing;

/// ================================================== ///
/// 加算
Quaternion MathQuaternion::Add(const Quaternion& q1, const Quaternion& q2) {
	
	Quaternion result;

	result.x = q1.x + q2.x;
	result.y = q1.y + q2.y;
	result.z = q1.z + q2.z;
	result.w = q1.w + q2.w;

	return result;
}

/// ================================================== ///
/// 減算
Quaternion MathQuaternion::Subtract(const Quaternion& q1, const Quaternion& q2) {

	Quaternion result;

	result.x = q1.x - q2.x;
	result.y = q1.y - q2.y;
	result.z = q1.z - q2.z;
	result.w = q1.w - q2.w;

	return result;
}

/// ================================================== ///
/// 積
Quaternion MathQuaternion::Multiply(const Quaternion& q1, const Quaternion& q2) {
    
	Quaternion result;

	result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	result.x = q1.y * q2.z - q1.z * q2.y + q2.w * q1.x + q1.w * q2.x;
	result.y = q1.z * q2.x - q1.x * q2.z + q2.w * q1.y + q1.w * q2.y;
	result.z = q1.x * q2.y - q1.y * q2.x + q2.w * q1.z + q1.w * q2.z;

	return result;
}

/// ================================================== ///
/// スカラー倍
Quaternion MathQuaternion::Multiply(const Quaternion& q, float s) {
	
	Quaternion result;

	result.x = q.x * s;
	result.y = q.y * s;
	result.z = q.z * s;
	result.w = q.w * s;

	return result;
}

/// ================================================== ///
/// 内積
float MathQuaternion::Dot(const Quaternion& q1, const Quaternion& q2) {

	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

/// ================================================== ///
/// 共役クォータニオン
Quaternion MathQuaternion::Conjugate(const Quaternion& q) {
	
	return { -q.x, -q.y, -q.z, q.w };
}

/// ================================================== ///
/// ノルム(長さ)
float MathQuaternion::Norm(const Quaternion& q) {
	
	return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

/// ================================================== ///
/// 正規化
Quaternion MathQuaternion::Normalize(const Quaternion& q) {
	
	float norm = Norm(q);

	// ノルムがゼロの場合は正規化できないため、単位クォータニオンを返す
	if (norm == 0.0f) { return Identity(); }

	Quaternion result;

	result.x = q.x / norm;
	result.y = q.y / norm;
	result.z = q.z / norm;
	result.w = q.w / norm;

	return result;
}

/// ================================================== ///
/// 単位クォータニオン
Quaternion MathQuaternion::Identity() {

	Quaternion result;

	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	result.w = 1.0f;

	return result;
}

/// ================================================== ///
/// 逆クォータニオン
Quaternion MathQuaternion::Inverse(const Quaternion& q) {

	// ノルムの二乗を計算
	float squaredNorm = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;

	// ノルムの二乗がゼロの場合は逆クォータニオンが定義されないため、単位クォータニオンを返す
	if (squaredNorm == 0.0f) { return Identity(); }

	// 共役クォータニオンを計算
	Quaternion conjugate = Conjugate(q);

	Quaternion result;

	// 逆クォータニオンは共役クォータニオンをノルムの二乗で割ったもの
	result = conjugate / squaredNorm;

	return result;
}

/// ================================================== ///
/// 任意軸回転クォータニオンの作成
Quaternion MathQuaternion::MakeRotateAxisAngle(const Vector3& axis, float angle) {
	
	float cosHalfAngle = cosf(angle / 2.0f);
	float sinHalfAngle = sinf(angle / 2.0f);

	// 回転軸を正規化
	Vector3 normalizedAxis = MathVector::Normalize(axis);

	Quaternion result;

	result.x = normalizedAxis.x * sinHalfAngle;
	result.y = normalizedAxis.y * sinHalfAngle;
	result.z = normalizedAxis.z * sinHalfAngle;
	result.w = cosHalfAngle;

	return result;
}

/// ================================================== ///
/// 球面線形補間
Quaternion MathQuaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t) {
	
	// 内積を計算
	float dot = Dot(q1, q2);

	// クォータニオンをコピー
	Quaternion targetQ2 = q2;

	// 内積が負の場合
	if (dot < 0.0f) {

		dot = -dot;
		
		// クォータニオンを反転
		targetQ2 = -q2;
	}

	// 内積が大きい場合 (なす角が小さい場合)
	if (dot > 1.0f - 1e-6f) {
		
		// 線形補間で十分
		return Lerp(q1, targetQ2, t);
	}

	// 内積から角度を求める
	float theta = acosf(dot);
	// 正弦を求める
	float sinTheta = sinf(theta);

	// 補間係数を計算
	float s1 = sinf((1.0f - t) * theta) / sinTheta;
	float s2 = sinf(t * theta) / sinTheta;

	Quaternion result;

	result = s1 * q1 + s2 * targetQ2;

	return result;
}

Quaternion MathQuaternion::operator+(const Quaternion& q1, const Quaternion& q2) {
	
	return Add(q1, q2);
}

Quaternion MathQuaternion::operator+(const Quaternion& q) {
	
	return q;
}

Quaternion MathQuaternion::operator-(const Quaternion& q1, const Quaternion& q2) {

	return Subtract(q1, q2);
}

Quaternion MathQuaternion::operator-(const Quaternion& q) {
	
	return { -q.x, -q.y, -q.z, -q.w };
}

Quaternion MathQuaternion::operator*(const Quaternion& q1, const Quaternion& q2) {
   
	return Multiply(q1, q2);
}

Quaternion MathQuaternion::operator*(const Quaternion& q, float s) {
	
	return Multiply(q, s);
}

Quaternion MathQuaternion::operator*(float s, const Quaternion& q) {
	
	return Multiply(q, s);
}

Quaternion MathQuaternion::operator/(const Quaternion& q, float s) {
	
	return Multiply(q, 1.0f / s);
}

Quaternion MathQuaternion::operator/(float s, const Quaternion& q) {
	
	return Multiply(Inverse(q), s);
}

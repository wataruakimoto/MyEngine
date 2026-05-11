#include "MathQuaternion.h"

#include <cmath>

using namespace Engine;

Quaternion MathQuaternion::Multiply(const Quaternion& q1, const Quaternion& q2) {
    
	Quaternion result;

	result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	result.x = q1.y * q2.z - q1.z * q2.y + q2.w * q1.x + q1.w * q2.x;
	result.y = q1.z * q2.x - q1.x * q2.z + q2.w * q1.y + q1.w * q2.y;
	result.z = q1.x * q2.y - q1.y * q2.x + q2.w * q1.z + q1.w * q2.z;

	return result;
}

Quaternion MathQuaternion::Conjugate(const Quaternion& q) {
	
	return { -q.x, -q.y, -q.z, q.w };
}

float MathQuaternion::Norm(const Quaternion& q) {
	
	return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

Quaternion MathQuaternion::Normalize(const Quaternion& q) {
	
	float norm = Norm(q);

	// ノルムがゼロの場合は正規化できないため、単位クォータニオンを返す
	if (norm == 0.0f) { return Identity(); }

	return { q.x / norm, q.y / norm, q.z / norm, q.w / norm };
}

Quaternion MathQuaternion::Identity() {

	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

Quaternion MathQuaternion::Inverse(const Quaternion& q) {

	// ノルムの二乗を計算
	float squaredNorm = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	
	// ノルムの二乗がゼロの場合は逆クォータニオンが定義されないため、単位クォータニオンを返す
	if (squaredNorm == 0.0f) { return Identity(); }

	// 共役クォータニオンを計算
	Quaternion conjugate = Conjugate(q);

	// 逆クォータニオンは共役クォータニオンをノルムの二乗で割ったもの
	return { conjugate.x / squaredNorm, conjugate.y / squaredNorm, conjugate.z / squaredNorm, conjugate.w / squaredNorm };
}

Quaternion MathQuaternion::operator*(const Quaternion& q1, const Quaternion& q2) {
   
	return Multiply(q1, q2);
}

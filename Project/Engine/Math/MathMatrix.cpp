#include "MathMatrix.h"
#include "MathVector.h"

#include <cassert>
#include <cmath>
#include <numbers>

using namespace Engine;

Matrix4x4 MathMatrix::Add(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 resultAdd = {};

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			resultAdd.m[y][x] = m1.m[y][x] + m2.m[y][x];
		}
	}

	return resultAdd;
}

Matrix4x4 MathMatrix::Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 resultSubtract = {};

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			resultSubtract.m[y][x] = m1.m[y][x] - m2.m[y][x];
		}
	}

	return resultSubtract;
}

Matrix4x4 MathMatrix::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 resultMultiply = {};

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			resultMultiply.m[y][x] = m1.m[y][0] * m2.m[0][x] + m1.m[y][1] * m2.m[1][x] + m1.m[y][2] * m2.m[2][x] + m1.m[y][3] * m2.m[3][x];
		}
	}

	return resultMultiply;
}

Matrix4x4 MathMatrix::Inverse(const Matrix4x4& m) {

	float determinant =
		m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]
		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	Matrix4x4 resultInverse = {};

	resultInverse.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) / determinant;

	resultInverse.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) / determinant;

	resultInverse.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) / determinant;

	resultInverse.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) / determinant;


	resultInverse.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2]
		+ m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) / determinant;

	resultInverse.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) / determinant;

	resultInverse.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2]
		+ m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) / determinant;

	resultInverse.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) / determinant;


	resultInverse.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1]
		- m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) / determinant;

	resultInverse.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1]
		+ m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) / determinant;

	resultInverse.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1]
		- m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) / determinant;

	resultInverse.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) / determinant;


	resultInverse.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1]
		+ m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) / determinant;

	resultInverse.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1]
		- m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) / determinant;

	resultInverse.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1]
		+ m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) / determinant;

	resultInverse.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1]
		- m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) / determinant;

	return resultInverse;
}

Matrix4x4 MathMatrix::Transpose(const Matrix4x4& m) {

	Matrix4x4 resultTranspose = {};

	resultTranspose.m[0][0] = m.m[0][0];
	resultTranspose.m[0][1] = m.m[1][0];
	resultTranspose.m[0][2] = m.m[2][0];
	resultTranspose.m[0][3] = m.m[3][0];

	resultTranspose.m[1][0] = m.m[0][1];
	resultTranspose.m[1][1] = m.m[1][1];
	resultTranspose.m[1][2] = m.m[2][1];
	resultTranspose.m[1][3] = m.m[3][1];

	resultTranspose.m[2][0] = m.m[0][2];
	resultTranspose.m[2][1] = m.m[1][2];
	resultTranspose.m[2][2] = m.m[2][2];
	resultTranspose.m[2][3] = m.m[3][2];

	resultTranspose.m[3][0] = m.m[0][3];
	resultTranspose.m[3][1] = m.m[1][3];
	resultTranspose.m[3][2] = m.m[2][3];
	resultTranspose.m[3][3] = m.m[3][3];

	return resultTranspose;
}

Matrix4x4 MathMatrix::MakeIdentity4x4() {

	Matrix4x4 resultIdentity = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return resultIdentity;
}

Matrix4x4 MathMatrix::MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 resultTranslate = {};

	resultTranslate.m[0][0] = 1.0f;
	resultTranslate.m[0][1] = 0.0f;
	resultTranslate.m[0][2] = 0.0f;
	resultTranslate.m[0][3] = 0.0f;

	resultTranslate.m[1][0] = 0.0f;
	resultTranslate.m[1][1] = 1.0f;
	resultTranslate.m[1][2] = 0.0f;
	resultTranslate.m[1][3] = 0.0f;

	resultTranslate.m[2][0] = 0.0f;
	resultTranslate.m[2][1] = 0.0f;
	resultTranslate.m[2][2] = 1.0f;
	resultTranslate.m[2][3] = 0.0f;

	resultTranslate.m[3][0] = translate.x;
	resultTranslate.m[3][1] = translate.y;
	resultTranslate.m[3][2] = translate.z;
	resultTranslate.m[3][3] = 1.0f;

	return resultTranslate;
}

Matrix4x4 MathMatrix::MakeScaleMatrix(const Vector3& scale) {

	Matrix4x4 resultScale = {};

	resultScale.m[0][0] = scale.x;
	resultScale.m[0][1] = 0.0f;
	resultScale.m[0][2] = 0.0f;
	resultScale.m[0][3] = 0.0f;

	resultScale.m[1][0] = 0.0f;
	resultScale.m[1][1] = scale.y;
	resultScale.m[1][2] = 0.0f;
	resultScale.m[1][3] = 0.0f;

	resultScale.m[2][0] = 0.0f;
	resultScale.m[2][1] = 0.0f;
	resultScale.m[2][2] = scale.z;
	resultScale.m[2][3] = 0.0f;

	resultScale.m[3][0] = 0.0f;
	resultScale.m[3][1] = 0.0f;
	resultScale.m[3][2] = 0.0f;
	resultScale.m[3][3] = 1.0f;

	return resultScale;
}

Matrix4x4 MathMatrix::MakeRotateYMatrix(float radian) {

	Matrix4x4 rotateY = { 0.0f };

	rotateY.m[0][0] = cos(radian);
	rotateY.m[0][2] = -sin(radian);
	rotateY.m[1][1] = 1.0f;
	rotateY.m[2][0] = sin(radian);
	rotateY.m[2][2] = cos(radian);
	rotateY.m[3][3] = 1.0f;

	return rotateY;
}

Matrix4x4 MathMatrix::MakeRotateZMatrix(float radian) {

	Matrix4x4 rotateZ = { 0.0f };

	rotateZ.m[0][0] = cos(radian);
	rotateZ.m[0][1] = sin(radian);
	rotateZ.m[1][0] = -sin(radian);
	rotateZ.m[1][1] = cos(radian);
	rotateZ.m[2][2] = 1.0f;
	rotateZ.m[3][3] = 1.0f;

	return rotateZ;
}

Matrix4x4 MathMatrix::MakeRotateMatrix(Vector3 radian) {

	Matrix4x4 rotateX = { 0.0f };

	rotateX.m[0][0] = 1.0f;
	rotateX.m[1][1] = cosf(radian.x);
	rotateX.m[1][2] = sinf(radian.x);
	rotateX.m[2][1] = -sinf(radian.x);
	rotateX.m[2][2] = cosf(radian.x);
	rotateX.m[3][3] = 1.0f;

	Matrix4x4 rotateY = { 0.0f };

	rotateY.m[0][0] = cosf(radian.y);
	rotateY.m[0][2] = -sinf(radian.y);
	rotateY.m[1][1] = 1.0f;
	rotateY.m[2][0] = sinf(radian.y);
	rotateY.m[2][2] = cosf(radian.y);
	rotateY.m[3][3] = 1.0f;

	Matrix4x4 rotateZ = { 0.0f };

	rotateZ.m[0][0] = cosf(radian.z);
	rotateZ.m[0][1] = sinf(radian.z);
	rotateZ.m[1][0] = -sinf(radian.z);
	rotateZ.m[1][1] = cosf(radian.z);
	rotateZ.m[2][2] = 1.0f;
	rotateZ.m[3][3] = 1.0f;

	Matrix4x4 resultRotate = { 0.0f };

	resultRotate = Multiply(rotateX, Multiply(rotateY, rotateZ));

	return resultRotate;
}

Matrix4x4 MathMatrix::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 rotateMatrix = { 0.0f };

	rotateMatrix = MakeRotateMatrix(rotate);

	Matrix4x4 resultAffine = { 0.0f };

	resultAffine.m[0][0] = scale.x * rotateMatrix.m[0][0];
	resultAffine.m[0][1] = scale.x * rotateMatrix.m[0][1];
	resultAffine.m[0][2] = scale.x * rotateMatrix.m[0][2];
	resultAffine.m[1][0] = scale.y * rotateMatrix.m[1][0];
	resultAffine.m[1][1] = scale.y * rotateMatrix.m[1][1];
	resultAffine.m[1][2] = scale.y * rotateMatrix.m[1][2];
	resultAffine.m[2][0] = scale.z * rotateMatrix.m[2][0];
	resultAffine.m[2][1] = scale.z * rotateMatrix.m[2][1];
	resultAffine.m[2][2] = scale.z * rotateMatrix.m[2][2];

	resultAffine.m[3][0] = translate.x;
	resultAffine.m[3][1] = translate.y;
	resultAffine.m[3][2] = translate.z;
	resultAffine.m[3][3] = 1.0f;

	return resultAffine;
}

Matrix4x4 MathMatrix::MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {

	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotate);

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	Matrix4x4 result = scaleMatrix * rotateMatrix * translateMatrix;

	return result;
}

Matrix4x4 MathMatrix::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {

	Matrix4x4 resultPerspectiveFov = {};

	resultPerspectiveFov.m[0][0] = (1 / aspectRatio) * (1 / tanf(fovY / 2));
	resultPerspectiveFov.m[1][1] = 1 / tanf(fovY / 2);
	resultPerspectiveFov.m[2][2] = farClip / (farClip - nearClip);
	resultPerspectiveFov.m[2][3] = 1.0f;
	resultPerspectiveFov.m[3][2] = -nearClip * farClip / (farClip - nearClip);

	return resultPerspectiveFov;
}

// 正射影行列
Matrix4x4 MathMatrix::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {

	Matrix4x4 resultOrthographic = {};

	resultOrthographic.m[0][0] = 2 / (right - left);
	resultOrthographic.m[1][1] = 2 / (top - bottom);
	resultOrthographic.m[2][2] = 1 / (farClip - nearClip);
	resultOrthographic.m[3][0] = (left + right) / (left - right);
	resultOrthographic.m[3][1] = (top + bottom) / (bottom - top);
	resultOrthographic.m[3][2] = nearClip / (nearClip - farClip);
	resultOrthographic.m[3][3] = 1.0f;

	return resultOrthographic;
}

Matrix4x4 MathMatrix::MakeViewportMatrix(float x, float y, float width, float height, float minZ, float maxZ) {

	Matrix4x4 resultViewport = {};

	resultViewport.m[0][0] = width / 2.0f;
	resultViewport.m[1][1] = height / 2.0f;
	resultViewport.m[2][2] = (maxZ - minZ) / 2.0f;
	resultViewport.m[3][0] = x + width / 2.0f;
	resultViewport.m[3][1] = y + height / 2.0f;
	resultViewport.m[3][2] = (maxZ + minZ) / 2.0f;
	resultViewport.m[3][3] = 1.0f;

	return resultViewport;
}

/// ================================================== ///
/// 任意軸回転行列の作成
Matrix4x4 MathMatrix::MakeRotateAxisAngle(const Vector3& axis, float radian) {

	float cosTheta = cosf(radian);
	float sinTheta = sinf(radian);

	Matrix4x4 result = MakeIdentity4x4();

	result.m[0][0] = axis.x * axis.x * (1.0f - cosTheta) + cosTheta;
	result.m[0][1] = axis.x * axis.y * (1.0f - cosTheta) + axis.z * sinTheta;
	result.m[0][2] = axis.x * axis.z * (1.0f - cosTheta) - axis.y * sinTheta;

	result.m[1][0] = axis.y * axis.x * (1.0f - cosTheta) - axis.z * sinTheta;
	result.m[1][1] = axis.y * axis.y * (1.0f - cosTheta) + cosTheta;
	result.m[1][2] = axis.y * axis.z * (1.0f - cosTheta) + axis.x * sinTheta;

	result.m[2][0] = axis.z * axis.x * (1.0f - cosTheta) + axis.y * sinTheta;
	result.m[2][1] = axis.z * axis.y * (1.0f - cosTheta) - axis.x * sinTheta;
	result.m[2][2] = axis.z * axis.z * (1.0f - cosTheta) + cosTheta;

	return result;
}

/// ================================================== ///
/// 2つの方向ベクトルから回転行列を作成
Matrix4x4 MathMatrix::DirectionToDirection(const Vector3& from, const Vector3& to) {

	// 外積で回転軸を求める
	Vector3 cross = MathVector::Cross(from, to);

	// 内積でcosθを求める
	float cosTheta = MathVector::Dot(from, to);
	// 外積の長さでsinθを求める
	float sinTheta = MathVector::Length(cross);

	// fromとtoがほとんど同じ向き(0°)な場合 (cosθ ≒ 1)
	if (cosTheta >= 1.0f - 1e-6f) {

		// 単位行列を返す
		return MakeIdentity4x4();
	}
	// fromとtoがほとんど逆向き(180°)な場合 (cosθ ≒ -1)
	else if (cosTheta <= -1.0f + 1e-6f) {

		// 外積が0ベクトルになり回転軸が定まらないので
		// fromと平行ではない軸との外積から直行する回転軸を求める

		// 直行する回転軸
		Vector3 axis;

		// fromのx成分とy成分がともに小さい(45°未満)なら
		if (std::abs(from.x) < 0.7071f || std::abs(from.y) < 0.7071f) {
			
			// x軸とは平行に近いので、z軸(0,0,1)との外積を回転軸とする
			axis = MathVector::Cross(from, { 0.0f, 0.0f, 1.0f });
		}
		// fromのx成分とz成分がともに小さい(45°未満)なら
		else if (std::abs(from.x) < 0.7071f || std::abs(from.z) < 0.7071f) {
			
			// x軸とは平行に近いので、y軸(0,1,0)との外積を回転軸とする
			axis = MathVector::Cross(from, { 0.0f, 1.0f, 0.0f });
		}

		// 180度回転行列を返す
		return MakeRotateAxisAngle(MathVector::Normalize(axis), std::numbers::pi_v<float>);
	}
	// それ以外の場合
	else {
		
		// 回転軸を正規化する
		Vector3 axis = MathVector::Normalize(cross);

		// cosθから角度を求める
		float angle = acosf(cosTheta);

		// 通常の回転行列を返す
		return MakeRotateAxisAngle(axis, angle);
	}
}

Matrix4x4 MathMatrix::MakeRotateMatrix(const Quaternion& q) {
	
	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float ww = q.w * q.w;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	Matrix4x4 result = MakeIdentity4x4();

	result.m[0][0] = ww + xx - yy - zz;
	result.m[0][1] = 2.0f * (xy + wz);
	result.m[0][2] = 2.0f * (xz - wy);

	result.m[1][0] = 2.0f * (xy - wz);
	result.m[1][1] = ww - xx + yy - zz;
	result.m[1][2] = 2.0f * (yz + wx);

	result.m[2][0] = 2.0f * (xz + wy);
	result.m[2][1] = 2.0f * (yz - wx);
	result.m[2][2] = ww - xx - yy + zz;

	return result;
}

//Matrix4x4 MathMatrix::operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
//
//	return Add(m1, m2);
//}
//
//Matrix4x4 MathMatrix::operator+(const Matrix4x4& m) {
//
//	return m;
//}
//
//Matrix4x4& MathMatrix::operator+=(Matrix4x4& m1, const Matrix4x4& m2) {
//
//	for (int x = 0; x < 4; x++) {
//		for (int y = 0; y < 4; y++) {
//
//			m1.m[y][x] += m2.m[y][x];
//		}
//	}
//
//	return m1;
//}
//
//Matrix4x4 MathMatrix::operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
//
//	return Subtract(m1, m2);
//}
//
//Matrix4x4 MathMatrix::operator-(const Matrix4x4& m) {
//
//	return m;
//}
//
//Matrix4x4& MathMatrix::operator-=(Matrix4x4& m1, const Matrix4x4& m2) {
//
//	for (int x = 0; x < 4; x++) {
//		for (int y = 0; y < 4; y++) {
//
//			m1.m[y][x] -= m2.m[y][x];
//		}
//	}
//
//	return m1;
//}
//
Matrix4x4 MathMatrix::operator*(const Matrix4x4& m1, const Matrix4x4& m2) {

	return Multiply(m1, m2);
}
//
//Matrix4x4 MathMatrix::operator*(const Matrix4x4& m) {
//
//	return m;
//}

Matrix4x4& MathMatrix::operator*=(Matrix4x4& m1, const Matrix4x4& m2) {

	m1 = Multiply(m1, m2);

	return m1;
}
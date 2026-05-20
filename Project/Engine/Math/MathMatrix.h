#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"
#include "Quaternion.h"

namespace Engine {

	/// <summary>
	/// 行列計算
	/// </summary>
	namespace MathMatrix {

		///=====================================================/// 
		/// 関数
		///=====================================================///

		// 行列の加法
		Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

		// 行列の減法
		Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

		// 行列の積
		Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

		// 逆行列
		Matrix4x4 Inverse(const Matrix4x4& m);

		// 転置行列
		Matrix4x4 Transpose(const Matrix4x4& m);

		// 単位行列の作成
		Matrix4x4 MakeIdentity4x4();

		// 平行移動行列
		Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

		// 拡大縮小行列
		Matrix4x4 MakeScaleMatrix(const Vector3& scale);

		// Y回転行列
		Matrix4x4 MakeRotateYMatrix(float radian);

		// Z回転行列
		Matrix4x4 MakeRotateZMatrix(float radian);

		// 3次元回転行列
		Matrix4x4 MakeRotateMatrix(Vector3 radian);

		// 3次元アフィン変換行列
		Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

		// 3次元アフィン変換行列（クォータニオン版）
		Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

		// 透視投影行列
		Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

		// 正射影行列
		Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

		// ビューポート行列
		Matrix4x4 MakeViewportMatrix(float x, float y, float width, float height, float minZ, float maxZ);

		/// <summary>
		/// 任意軸回転行列の作成
		/// </summary>
		/// <param name="axis">回転軸</param>
		/// <param name="radian">回転角（ラジアン）</param>
		/// <returns>回転行列</returns>
		Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float radian);

		/// <summary>
		/// 2つの方向ベクトルから回転行列を作成
		/// </summary>
		/// <param name="from">開始方向ベクトル</param>
		/// <param name="to">目標方向ベクトル</param>
		/// <returns>回転行列</returns>
		Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);
		
		/// <summary>
		/// 回転行列の作成
		/// </summary>
		/// <param name="q">クォータニオン</param>
		/// <returns>回転行列</returns>
		Matrix4x4 MakeRotateMatrix(const Quaternion& q);

		///=====================================================/// 
		/// オペレーター演算子
		///=====================================================///

		//Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);

		//Matrix4x4 operator+(const Matrix4x4& m);

		//Matrix4x4& operator+=(Matrix4x4& m1, const Matrix4x4& m2);

		//Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);

		//Matrix4x4 operator-(const Matrix4x4& m);

		//Matrix4x4& operator-=(Matrix4x4& m1, const Matrix4x4& m2);

		Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

		//Matrix4x4 operator*(const Matrix4x4& m);

		Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);
	};
}
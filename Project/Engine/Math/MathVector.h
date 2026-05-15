#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

namespace Engine {

	/// <summary>
	/// ベクトル計算
	/// </summary>
	namespace MathVector {

		/// ================================================== ///
		/// 関数
		/// ================================================== ///

		/// <summary>
		/// 加算
		/// </summary>
		/// <param name="v1">左オペランド</param>
		/// <param name="v2">右オペランド</param>
		/// <returns>加算結果</returns>
		Vector2 Add(const Vector2& v1, const Vector2& v2);

		/// <summary>
		/// 加算
		/// </summary>
		/// <param name="v1">左オペランド</param>
		/// <param name="v2">右オペランド</param>
		/// <returns>加算結果</returns>
		Vector3 Add(const Vector3& v1, const Vector3& v2);

		/// <summary>
		/// 加算
		/// </summary>
		/// <param name="v1">左オペランド</param>
		/// <param name="v2">右オペランド</param>
		/// <returns>加算結果</returns>
		Vector4 Add(const Vector4& v1, const Vector4& v2);

		/// <summary>
		/// 減算
		/// </summary>
		/// <param name="v1">左オペランド</param>
		/// <param name="v2">右オペランド</param>
		/// <returns>減算結果</returns>
		Vector2 Subtract(const Vector2& v1, const Vector2& v2);

		/// <summary>
		/// 減算
		/// </summary>
		/// <param name="v1">左オペランド</param>
		/// <param name="v2">右オペランド</param>
		/// <returns>減算結果</returns>
		Vector3 Subtract(const Vector3& v1, const Vector3& v2);

		/// <summary>
		/// 減算
		/// </summary>
		/// <param name="v1">左オペランド</param>
		/// <param name="v2">右オペランド</param>
		/// <returns>減算結果</returns>
		Vector4 Subtract(const Vector4& v1, const Vector4& v2);

		/// <summary>
		/// スカラー倍
		/// </summary>
		/// <param name="v">ベクトル</param>
		/// <param name="s">スカラー値</param>
		/// <returns>スカラー倍結果</returns>
		Vector2 Multiply(const Vector2& v, float s);

		/// <summary>
		/// スカラー倍
		/// </summary>
		/// <param name="v">ベクトル</param>
		/// <param name="s">スカラー値</param>
		/// <returns>スカラー倍結果</returns>
		Vector3 Multiply(const Vector3& v, float s);
		
		/// <summary>
		/// スカラー倍
		/// </summary>
		/// <param name="v">ベクトル</param>
		/// <param name="s">スカラー値</param>
		/// <returns>スカラー倍結果</returns>
		Vector4 Multiply(const Vector4& v, float s);

		// 内積
		float Dot(const Vector2& v1, const Vector2& v2);
		float Dot(const Vector3& v1, const Vector3& v2);
		float Dot(const Vector4& v1, const Vector4& v2);

		// クロス
		Vector2 Cross(const Vector2& v1, const Vector2& v2);
		Vector3 Cross(const Vector3& v1, const Vector3& v2);

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

		/// <summary>
		/// ベクトルをクォータニオンで回転させる
		/// </summary>
		/// <param name="v">ベクトル</param>
		/// <param name="q">クォータニオン</param>
		/// <returns>回転ベクトル</returns>
		Vector3 RotateVector(const Vector3& v, const Quaternion& q);

		/// ================================================== ///
		/// 演算子オーバーロード
		/// ================================================== ///

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

		Vector2 operator*(const Vector2& v, float s);
		Vector2 operator*(float s, const Vector2& v);
		Vector2& operator*=(Vector2& v, float s);

		Vector3 operator*(const Vector3& v, float s);
		Vector3 operator*(float s, const Vector3& v);
		Vector3& operator*=(Vector3& v, float s);

		Vector4 operator*(const Vector4& v, float s);
		Vector4 operator*(float s, const Vector4& v);
		Vector4& operator*=(Vector4& v, float s);

		/// ===== 除算 ===== ///

		Vector2 operator/(const Vector2& v, float s);
		Vector2& operator/=(Vector2& v, float s);

		Vector3 operator/(const Vector3& v, float s);
		Vector3& operator/=(Vector3& v, float s);

		Vector4 operator/(const Vector4& v, float s);
		Vector4& operator/=(Vector4& v, float s);
	}
}
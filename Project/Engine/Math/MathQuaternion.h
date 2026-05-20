#pragma once

#include "Quaternion.h"
#include "Vector3.h"

namespace Engine {

	/// <summary>
	/// クォータニオン計算
	/// </summary>
	namespace MathQuaternion {

		/// ================================================== ///
		/// 関数
		/// ================================================== ///
		
		/// <summary>
		/// 加算
		/// </summary>
		/// <param name="q1">左オペランド</param>
		/// <param name="q2">右オペランド</param>
		/// <returns>加算の結果</returns>
		Quaternion Add(const Quaternion& q1, const Quaternion& q2);

		/// <summary>
		/// 減算
		/// </summary>
		/// <param name="q1">左オペランド</param>
		/// <param name="q2">右オペランド</param>
		/// <returns>減算の結果</returns>
		Quaternion Subtract(const Quaternion& q1, const Quaternion& q2);

		/// <summary>
		/// 積
		/// </summary>
		/// <param name="q1">左オペランド</param>
		/// <param name="q2">右オペランド</param>
		/// <returns>積の結果</returns>
		Quaternion Multiply(const Quaternion& q1, const Quaternion& q2);

		/// <summary>
		/// スカラー倍
		/// </summary>
		/// <param name="q">クォータニオン</param>
		/// <param name="s">スカラー</param>
		/// <returns>スカラー倍されたクォータニオン</returns>
		Quaternion Multiply(const Quaternion& q, float s);

		/// <summary>
		/// 内積
		/// </summary>
		/// <param name="q1">クォータニオン</param>
		/// <param name="q2">クォータニオン</param>
		/// <returns>内積</returns>
		float Dot(const Quaternion& q1, const Quaternion& q2);

		/// <summary>
		/// 共役クォータニオン
		/// </summary>
		/// <param name="q">クォータニオン</param>
		/// <returns>共役クォータニオン</returns>
		Quaternion Conjugate(const Quaternion& q);

		/// <summary>
		/// ノルム (長さ)
		/// </summary>
		/// <param name="q">クォータニオン</param>
		/// <returns>ノルム</returns>
		float Norm(const Quaternion& q);

		/// <summary>
		/// 正規化
		/// </summary>
		/// <param name="q">クォータニオン</param>
		/// <returns>正規化されたクォータニオン</returns>
		Quaternion Normalize(const Quaternion& q);

		/// <summary>
		/// 単位クォータニオン
		/// </summary>
		/// <returns>単位クォータニオン</returns>
		Quaternion Identity();

		/// <summary>	
		/// 逆クォータニオン
		/// </summary>
		/// <param name="q">クォータニオン</param>
		/// <returns>逆クォータニオン</returns>
		Quaternion Inverse(const Quaternion& q);

		/// <summary>
		/// 任意軸回転クォータニオンの作成
		/// </summary>
		/// <param name="axis">回転軸</param>
		/// <param name="angle">回転角度</param>
		/// <returns>回転クォータニオン</returns>
		Quaternion MakeRotateAxisAngle(const Vector3& axis, float angle);

		/// <summary>
		/// 球面線形補間
		/// </summary>
		/// <param name="start">開始クォータニオン</param>
		/// <param name="end">終了クォータニオン</param>
		/// <param name="t">補間パラメータ (0.0 から 1.0)</param>
		/// <returns>補間されたクォータニオン</returns>
		Quaternion Slerp(const Quaternion& start, const Quaternion& end, float t);

		/// <summary>
		/// クォータニオンをオイラー角に変換
		/// </summary>
		/// <param name="q">クォータニオン</param>
		/// <returns>オイラー角</returns>
		Vector3 QuaternionToEuler(const Quaternion& q);

		/// <summary>
		/// オイラー角をクォータニオンに変換
		/// </summary>
		/// <param name="v">オイラー角</param>
		/// <returns>クォータニオン</returns>
		Quaternion EulerToQuaternion(const Vector3& v);

		/// ================================================== ///
		/// オペレーター演算子
		/// ================================================== ///
		
		Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
		Quaternion operator+(const Quaternion& q);
		Quaternion operator+=(Quaternion& q1, const Quaternion& q2);

		Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
		Quaternion operator-(const Quaternion& q);
		Quaternion operator-=(Quaternion& q1, const Quaternion& q2);

		Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
		Quaternion operator*=(Quaternion& q1, const Quaternion& q2);

		Quaternion operator*(const Quaternion& q, float s);
		Quaternion operator*(float s, const Quaternion& q);
		Quaternion operator*=(Quaternion& q, float s);

		Quaternion operator/(const Quaternion& q, float s);
		Quaternion operator/(float s, const Quaternion& q);
		Quaternion operator/=(Quaternion& q, float s);
	}
}
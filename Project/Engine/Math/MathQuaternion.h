#pragma once

#include "Quaternion.h"

namespace Engine {

	/// <summary>
	/// クォータニオン計算
	/// </summary>
	namespace MathQuaternion {

		/// ================================================== ///
		/// 関数
		/// ================================================== ///

		/// <summary>
		/// 積
		/// </summary>
		/// <param name="q1">左オペランド</param>
		/// <param name="q2">右オペランド</param>
		/// <returns>積の結果</returns>
		Quaternion Multiply(const Quaternion& q1, const Quaternion& q2);

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

		/// ================================================== ///
		/// オペレーター演算子
		/// ================================================== ///

		Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
	}
}
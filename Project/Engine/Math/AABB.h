#pragma once

#include "Vector3.h"
#include "MathVector.h"

namespace Engine {

	using namespace MathVector;

	// 軸平行境界ボックスの構造体
	struct AABB {
		Vector3 min; // 最小座標
		Vector3 max; // 最大座標
	};

	/// <summary>
	/// AABBの作成関数
	/// </summary>
	/// <param name="min">最小</param>
	/// <param name="max">最大</param>
	/// <returns>AABB</returns>
	inline AABB CreateAABB(const Vector3& min, const Vector3& max) {
		return AABB{ min, max };
	};

	/// <summary>
	/// AABBの作成関数
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="halfSize">半分の大きさ</param>
	/// <returns>AABB</returns>
	inline AABB CreateAABBFromCenter(const Vector3& center, const Vector3& halfSize) {
		return AABB{ center - halfSize, center + halfSize };
	};
}
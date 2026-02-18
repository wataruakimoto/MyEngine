#pragma once

#include "Vector3.h"
#include "MathVector.h"

namespace Engine {

	// 平面の構造体
	struct Plane {
		Vector3 normal; // 法線
		float distance; // 距離
	};

	/// <summary>
	/// 平面の作成関数
	/// </summary>
	/// <param name="normal">法線</param>
	/// <param name="distance">距離</param>
	/// <returns></returns>
	inline Plane CreatePlane(const Vector3& normal, float distance) {
		return Plane{ normal, distance };
	};

	/// <summary>
	/// 平面の作成関数
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="normal">法線</param>
	/// <returns></returns>
	inline Plane CreatePlane(const Vector3& point, const Vector3& normal) {
		
		Plane plane;
		plane.normal = MathVector::Normalize(normal);
		plane.distance = MathVector::Dot(plane.normal, point);

		return plane;
	};
}
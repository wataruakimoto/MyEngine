#pragma once

#include "Vector3.h"

namespace Engine {

	// 球の構造体
	struct Sphere {
		Vector3 center; // 中心座標
		float radius; // 半径
	};

	// 球の作成関数
	inline Sphere CreateSphere(const Vector3& center, float radius) {
		return Sphere{ center, radius };
	};
}
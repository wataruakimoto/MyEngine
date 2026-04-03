#pragma once

#include "Vector3.h"

namespace Engine {

	/// <summary>
	/// 楕円形
	/// </summary>
	struct Ellipsoid {
		Vector3 center; // 中心座標
		Vector3 radius; // 半径
	};
}
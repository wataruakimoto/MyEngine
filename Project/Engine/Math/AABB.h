#pragma once

#include "Vector3.h"

namespace Engine {

	struct AABB {
		Vector3 min; // 最小座標
		Vector3 max; // 最大座標
	};
}
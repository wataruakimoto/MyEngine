#pragma once

#include "Vector3.h"

namespace Engine {

	struct Plane {
		Vector3 normal; // 法線
		float distance; // 距離
	};
}
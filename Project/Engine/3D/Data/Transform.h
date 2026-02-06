#pragma once

#include "Vector3.h"

namespace Engine {

	// 変換データ
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};
}
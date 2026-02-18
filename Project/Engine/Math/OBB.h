#pragma once

#include "Vector3.h"

namespace Engine {

	struct OBB {
		Vector3 center; // 中心座標
		Vector3 orientations[3]; // 各軸の方向ベクトル
		Vector3 halfSize; // 中心から面までの距離 (座標軸方向の半分の長さ)
	};
}
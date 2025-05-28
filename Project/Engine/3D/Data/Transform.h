#pragma once
#include "math/Vector3.h"
#include "math/Matrix4x4.h"

// 変換データ
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// 座標変換データ
struct TransformationData {
	Matrix4x4 WVP;
};

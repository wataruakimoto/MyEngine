#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

/// <summary>
/// イージング関数ライブラリ
/// 参考: https://easings.net/ja :イージング関数チートシート
/// </summary>
namespace Easing {

	/// ===== 線形補間 ===== ///

	float Lerp(float start, float end, float t);

	Vector2 Lerp(const Vector2& start, const Vector2& end, float t);

	Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

	Vector4 Lerp(const Vector4& start, const Vector4& end, float t);

	/// ===== In ゆっくり始まって速く終わる ===== ///

	float EaseInCubic(float t);

	/// ===== Out 速く始まってゆっくり終わる ===== ///

	float EaseOutQuad(float t);

	float EaseOutCubic(float t);

	float EaseOutQuart(float t);

	float EaseOutBounce(float t);

	/// ===== InOut 滑らかな往復運動 ===== ///

	float EaseInOutQuad(float t);
}

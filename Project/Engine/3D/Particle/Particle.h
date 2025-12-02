#pragma once

#include "Data/Transform.h"
#include "Vector3.h"
#include "Vector4.h"

// パーティクル
struct Particle {

	// 設定項目
	Transform transform = { {1.0f,1.0f,1.0f} }; // デフォルトの変換データ(スケールだけ1.0)
    Vector3 velocity = { 0.0f, 0.0f, 0.0f }; // デフォルトの速度(動かさない)
    Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f }; // デフォルトの色(白)
	float lifeTime = 1.0f; // デフォルトの寿命(1秒)
    float currentTime = 0.0f; // 現在の時間
	bool useLifeTime = true; // 生存時間を適応するかどうか
	bool useBillboard = true; // ビルボードするかどうか

    // ランダム化フラグ
    bool randomizeScale = false;
	bool randomizeRotate = false;
	bool randomizeTranslate = false;
    bool randomizeVelocity = false;
    bool randomizeColor = false;
    bool randomizeLifeTime = false;

	// ランダムの範囲
	Vector3 randomScaleMin = { -1.0f, -1.0f, -1.0f };
	Vector3 randomScaleMax = { 1.0f, 1.0f, 1.0f };
	Vector3 randomRotateMin = { -1.0f, -1.0f, -1.0f };
	Vector3 randomRotateMax = { 1.0f, 1.0f, 1.0f };
	Vector3 randomTranslateMin = { -1.0f, -1.0f, -1.0f };
	Vector3 randomTranslateMax = { 1.0f, 1.0f, 1.0f };
	Vector3 randomVelocityMin = { -1.0f, -1.0f, -1.0f };
	Vector3 randomVelocityMax = { 1.0f, 1.0f, 1.0f };
	Vector4 randomColorMin = { 0.0f, 0.0f, 0.0f, 0.0f };
	Vector4 randomColorMax = { 1.0f, 1.0f, 1.0f, 1.0f };
	float randomLifeTimeMin = 0.0f;
	float randomLifeTimeMax = 1.0f;
};

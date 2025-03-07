#pragma once
#include "math/Vector3.h"
#include "Data/Transform.h"

#include <string>

/// === パーティクル発生機 === ///
class ParticleEmitter {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParticleEmitter(const std::string& name, const Transform& transform, uint32_t count, float frequency);

	/// <summary>
	/// パーティクル発生
	/// </summary>
	void Emit();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 名前
	std::string name;

	// 位置
	Transform transform;

	// 発生数
	uint32_t count;

	// 発生頻度
	float frequency;

	// 頻度用時刻
	float frequencyTime = 0.0f;

	// Δt
	const float kDeltaTime = 1.0f / 60.0f;
};
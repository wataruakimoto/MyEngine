#pragma once

#include "Data/Transform.h"
#include "Particle.h"

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
	ParticleEmitter(const std::string& name, const Transform& transform, uint32_t count, float frequency, Particle setting);

	/// <summary>
	/// パーティクル発生
	/// </summary>
	void Emit();

	void ShowImGui(const char* name);

	void SetTransform(const Transform& transform) { this->transform = transform; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 名前
	std::string particleName;

	// 位置
	Transform transform;

	// 発生数
	uint32_t count;

	// 発生頻度
	float frequency;

	// 頻度用時刻
	float frequencyTime;

	// Δt
	const float kDeltaTime = 1.0f / 60.0f;

	// 設定項目
	Particle settings;
};
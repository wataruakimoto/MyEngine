#pragma once

#include "BaseScene.h"
#include "Camera.h"
#include "Particle/ParticleCommon.h"
#include "Particle/ParticleSystem.h"
#include "Particle/ParticleEmitter.h"
#include "Data/Transform.h"

#include <memory>

/// ===== デバッグシーン ===== ///
class DebugScene : public BaseScene {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui() override;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// カメラのポインタ
	std::unique_ptr<Camera> camera = nullptr;

	// パーティクルシステムのインスタンス
	ParticleSystem* particleSystem = ParticleSystem::GetInstance();

	// パーティクル共通のインスタンス
	ParticleCommon* particleCommon = ParticleCommon::GetInstance();

	// エミッターのポインタ
	std::unique_ptr <ParticleEmitter> particleEmitter = nullptr;

	// エミッタ変換データ
	Transform emitterTransform = {};

	// パーティクル設定
	Particle particleSetting = {};
};
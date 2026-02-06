#pragma once

#include "BaseScene.h"
#include "Camera.h"
#include "Particle/ParticleCommon.h"
#include "Particle/ParticleManager.h"
#include "Particle/ParticleEmitter.h"
#include "OffscreenRendering/FilterManager.h"

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
	/// フィルター適応のある描画
	/// </summary>
	void DrawFiltered() override;

	/// <summary>
	/// フィルター適応のない描画
	/// </summary>
	void DrawUnfiltered() override;

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
	std::unique_ptr<Engine::Camera> camera = nullptr;

	// パーティクル共通のインスタンス
	Engine::ParticleCommon* particleCommon = Engine::ParticleCommon::GetInstance();

	// パーティクルマネージャーのインスタンス
	Engine::ParticleManager* particleManager = Engine::ParticleManager::GetInstance();

	// エミッターのポインタ
	std::unique_ptr<Engine::ParticleEmitter> particleEmitterRed = nullptr;
	std::unique_ptr<Engine::ParticleEmitter> particleEmitterBlue = nullptr;

	// フィルターマネージャのインスタンス
	Engine::FilterManager* filterManager;
};
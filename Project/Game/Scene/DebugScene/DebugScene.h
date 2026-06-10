#pragma once

#include "BaseScene.h"
#include "Camera.h"
#include "Particle/ParticleEmitter.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///

	class SceneManager;
	class ParticleRenderer;
	class ParticleManager;
}

/// ===== デバッグシーン ===== ///
class DebugScene : public BaseScene {

/// ================================================== ///
/// メンバ関数
/// ================================================== ///
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

/// ================================================== ///
/// メンバ変数
/// ================================================== ///
private:

	// カメラのポインタ
	std::unique_ptr<Engine::Camera> camera = nullptr;

	// シーンマネージャのポインタ
	Engine::SceneManager* sceneManager = nullptr;

	// パーティクルレンダラーのポインタ
	Engine::ParticleRenderer* particleRenderer = nullptr;

	// パーティクルマネージャのポインタ
	Engine::ParticleManager* particleManager = nullptr;

	// パーティクルエミッターのポインタ
	std::unique_ptr<Engine::ParticleEmitter> emitter = nullptr;
	std::unique_ptr<Engine::ParticleEmitter> emitterB = nullptr;
};
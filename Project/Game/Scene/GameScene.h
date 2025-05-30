#pragma once

#include "BaseScene.h"
#include "3d/Particle/ParticleEmitter.h"
#include "camera/Camera.h"
#include <memory>

/// ===== ゲームシーン ===== ///
class GameScene : public BaseScene {

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

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// カメラのポインタ
	std::unique_ptr<Camera> camera = nullptr;

	// 爆発エフェクト
	std::unique_ptr <ParticleEmitter> explosionEmitter = nullptr;
	Transform explosionEmitterTransform = {}; // 初期位置とスケール
	Particle explosionParticleSetting = {}; // パーティクルの設定

	// フラッシュエフェクト
	std::unique_ptr <ParticleEmitter> flashEmitter = nullptr;
	Transform flashEmitterTransform = {}; // 初期位置とスケール
	Particle flashParticleSetting = {}; // パーティクルの設定

	// スモークエフェクト
	std::unique_ptr <ParticleEmitter> smokeEmitter = nullptr;
	Transform smokeEmitterTransform = {}; // 初期位置とスケール
	Particle smokeParticleSetting = {}; // パーティクルの設定
};
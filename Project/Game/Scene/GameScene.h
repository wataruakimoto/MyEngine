#pragma once

#include "BaseScene.h"
#include "3d/Model.h"
#include "3d/Object3d.h"
#include "3d/Particle/ParticleEmitter.h"
#include "camera/Camera.h"

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

	// モデルのポインタ
	std::unique_ptr <Model> modelMonsterBall = nullptr;

	std::unique_ptr <Model> modelTerrain = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr <Object3d> monsterBall = nullptr;

	std::unique_ptr <Object3d> terrain = nullptr;

	// パーティクル発生機
	std::unique_ptr <ParticleEmitter> particleEmitter1 = nullptr;
	std::unique_ptr <ParticleEmitter> particleEmitter2 = nullptr;
	std::unique_ptr <ParticleEmitter> particleEmitter3 = nullptr;

	Transform EmitterTransform1 = {};
	Transform EmitterTransform2 = {};
	Transform EmitterTransform3 = {};

	Particle particleSetting1 = {};
	Particle particleSetting2 = {};
	Particle particleSetting3 = {};
};
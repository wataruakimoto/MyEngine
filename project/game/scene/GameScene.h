#pragma once

#include "BaseScene.h"
#include "3d/Model.h"
#include "3d/Object3d.h"
#include "3d/ParticleEmitter.h"
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
	Camera* camera = nullptr;

	// モデルのポインタ
	Model* modelMonsterBall = nullptr;

	Model* modelTerrain = nullptr;

	// 3Dオブジェクトのポインタ
	Object3d* monsterBall = nullptr;

	Object3d* terrain = nullptr;

	// パーティクル発生機
	ParticleEmitter* particleEmitter = nullptr;
	ParticleEmitter* particleEmitter2 = nullptr;
};
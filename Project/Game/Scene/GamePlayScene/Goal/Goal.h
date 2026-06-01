#pragma once

#include "Model/Model.h"
#include "Object/Object3d.h"
#include "WorldTransform.h"

/// ===== ゴール ===== ///
class Goal {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void ShowImGui();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	Engine::WorldTransform& GetWorldTransform() { return worldTransform_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルのポインタ
	std::unique_ptr<Engine::Model> model_ = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr<Engine::Object3d> object_ = nullptr;

	// ワールド変換
	Engine::WorldTransform worldTransform_;
};


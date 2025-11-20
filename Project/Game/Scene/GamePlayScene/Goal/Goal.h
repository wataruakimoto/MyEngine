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

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルのポインタ
	std::unique_ptr<Model> model_ = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr<Object3d> object_ = nullptr;

	// ワールド変換
	WorldTransform worldTransform_;
};


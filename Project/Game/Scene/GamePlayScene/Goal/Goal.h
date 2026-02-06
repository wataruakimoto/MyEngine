#pragma once

#include "Model/Model.h"
#include "Object/Object3d.h"
#include "WorldTransform.h"

/// ===== 前方宣言 ===== ///
class Player;

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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="killCount_"></param>
	void CheckGateCollision(int killCount);

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	Engine::WorldTransform& GetWorldTransform() { return worldTransform_; }

	Engine::WorldTransform& GetGateWorldTransform() { return gateWorldTransform_; }

	const int GetNormaCount() const { return kNormaCount; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	void SetPlayer(Player* player) { player_ = player; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルのポインタ
	std::unique_ptr<Engine::Model> model_ = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr<Engine::Object3d> object_ = nullptr;

	// ゲート用のモデル
	std::unique_ptr<Engine::Model> gateModel_ = nullptr;

	// ゲート用の3Dオブジェクト
	std::unique_ptr<Engine::Object3d> gateObject_ = nullptr;

	// ワールド変換
	Engine::WorldTransform worldTransform_;

	// ゲートのワールド変換
	Engine::WorldTransform gateWorldTransform_;

	// ノルマの倒した数
	const int kNormaCount = 5;

	// ロックされているかどうか
	bool isLocked = true;

	// アルファ値
	float alpha_ = 1.0f;

	// プレイヤーの借りポインタ
	Player* player_ = nullptr;
};


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

	WorldTransform& GetWorldTransform() { return worldTransform_; }

	WorldTransform& GetGateWorldTransform() { return gateWorldTransform_; }

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
	std::unique_ptr<Model> model_ = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr<Object3d> object_ = nullptr;

	// ゲート用のモデル
	std::unique_ptr<Model> gateModel_ = nullptr;

	// ゲート用の3Dオブジェクト
	std::unique_ptr<Object3d> gateObject_ = nullptr;

	// ワールド変換
	WorldTransform worldTransform_;

	// ゲートのワールド変換
	WorldTransform gateWorldTransform_;

	// ノルマの倒した数
	const int kNormaCount = 5;

	// ロックされているかどうか
	bool isLocked = true;

	// アルファ値
	float alpha_ = 1.0f;

	// プレイヤーの借りポインタ
	Player* player_ = nullptr;
};


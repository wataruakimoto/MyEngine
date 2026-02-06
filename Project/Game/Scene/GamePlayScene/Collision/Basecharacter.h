#pragma once

#include "Collider.h"
#include "WorldTransform.h"

///-------------------------------------------/// 
/// キャラクター基底クラス
///-------------------------------------------///
class BaseCharacter : public Collider {

public:

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void OnCollision([[maybe_unused]] Collider* other) override = 0;

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	// 中心座標の取得
	const Engine::Vector3 GetCenterPosition() const { return worldTransform_.GetWorldPosition(); }

	/// <summary>
	/// ワールド変換のゲッター
	/// </summary>
	/// <returns></returns>
	Engine::WorldTransform& GetWorldTransform() { return worldTransform_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// ワールド変換のセッター
	/// </summary>
	/// <param name="worldTransform"></param>
	void SetWorldTransform(const Engine::WorldTransform& worldTransform) { worldTransform_ = worldTransform; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
protected:

	// ワールド変換
	Engine::WorldTransform worldTransform_;
};

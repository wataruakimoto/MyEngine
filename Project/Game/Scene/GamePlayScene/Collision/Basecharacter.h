#pragma once
#include "Collider.h"
#include "WorldTransform/WorldTransform.h"

///-------------------------------------------/// 
/// キャラクター基底クラス
///-------------------------------------------///
class Basecharacter : public Collider {

public:

	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

	virtual void OnCollision([[maybe_unused]] Collider* other) override;

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	// 中心座標の取得
	virtual const Vector3 GetCenterPosition() const override;

	/// <summary>
	/// ワールド変換のゲッター
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// ワールド変換のセッター
	/// </summary>
	/// <param name="worldTransform"></param>
	void SetWorldTransform(const WorldTransform& worldTransform) { worldTransform_ = worldTransform; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
protected:

	// ワールド変換
	WorldTransform worldTransform_;
};

#pragma once
#include "Collider.h"

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

protected:

	// 変形データ
	Transform transform_;
};

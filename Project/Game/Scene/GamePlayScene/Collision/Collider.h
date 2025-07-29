#pragma once
#include "math/Vector3.h"
#include "3D/Data/Transform.h"

#include <cstdint>

///-------------------------------------------/// 
/// 衝突判定オブジェクト
///-------------------------------------------///
class Collider {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	// 仮想デストラクタ
	virtual ~Collider() = default;

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 衝突時のコールバック関数
	virtual void OnCollision([[maybe_unused]] Collider* other) = 0;

	// 中心座標を取得
	virtual const Vector3 GetCenterPosition() const = 0;

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	// 半径のゲッター
	const float GetRadius() const { return radius_; }

	// 種別IDのゲッター
	uint32_t GetTypeID() const { return typeID_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	// 半径のセッター
	void SetRadius(float radius) { radius_ = radius; }

	// 種別IDのセッター
	void SetTypeID(uint32_t typeID) { typeID_ = typeID; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 衝突半径
	float radius_ = 1.0f;

	// 種別ID
	uint32_t typeID_ = 0u;
};

#pragma once

#include "Vector3.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "OBB.h"

#include <cstdint>
#include <variant>

namespace Engine {

	///-------------------------------------------/// 
	/// 衝突判定オブジェクト
	///-------------------------------------------///
	class Collider {

		///-------------------------------------------/// 
		/// 型の定義
		///-------------------------------------------///
	public:

		// 衝突形状なしの型
		struct None {};

		// 衝突形状の型 5種類のいずれか1つを保持
		using CollisionShape = std::variant<None, Sphere, Plane, AABB, OBB>;

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

		///-------------------------------------------/// 
		/// ゲッター
		///-------------------------------------------///
	public:

		// 形状のゲッター
		const CollisionShape& GetShape() const { return shape_; }

		// 種別IDのゲッター
		uint32_t GetTypeID() const { return typeID_; }

		///-------------------------------------------/// 
		/// セッター
		///-------------------------------------------///
	public:

		// 形状のセッター
		void SetShape(const CollisionShape& shape);

		// 種別IDのセッター
		void SetTypeID(uint32_t typeID) { typeID_ = typeID; }

		// 球のセッター
		void SetSphere(const Sphere& sphere);

		// 平面のセッター
		void SetPlane(const Plane& plane);

		// AABBのセッター
		void SetAABB(const AABB& aabb);

		// OBBのセッター
		void SetOBB(const OBB& obb);

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		// 形状
		CollisionShape shape_;

		// 種別ID
		uint32_t typeID_ = 0u;
	};
}
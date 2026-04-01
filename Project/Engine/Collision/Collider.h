#pragma once

#include "WorldTransform.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "OBB.h"

#include <cstdint>
#include <variant>
#include <functional>

namespace Engine {

	/// ========== 前方宣言 ========== ///

	class LineManager;

	/// <summary>
	/// 当たり判定クラス
	/// </summary>
	class Collider {

	/// ================================================== ///
	/// 構造体
	/// ================================================== ///
	public:

		// 衝突形状なしの型
		struct None {};

		// 衝突形状の型 5種類のいずれか1つを保持
		using CollisionShape = std::variant<None, Sphere, Plane, AABB, OBB>;
		
	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="shape">判定の形状</param>
		/// <param name="typeID">種別ID</param>
		Collider(const CollisionShape& shape, uint32_t typeID);

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Collider();

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// ImGui表示
		/// </summary>
		void ShowImGui();

		/// <summary>
		/// 衝突時のコールバック関数の呼び出し
		/// </summary>
		/// <param name="other">衝突相手</param>
		void OnCollision(Collider* other);

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
	public:

		/// <summary>
		/// ワールド座標変換の取得
		/// </summary>
		/// <returns>ワールド座標変換</returns>
		WorldTransform& GetWorldTransform() { return worldTransform_; }

		// 形状のゲッター
		const CollisionShape& GetShape() const { return shape_; }

		// 種別IDのゲッター
		uint32_t GetTypeID() const { return typeID_; }

	/// ================================================== ///
	/// セッター
	/// ================================================== ///
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

		/// <summary>
		/// 衝突時のコールバック関数の設定
		/// </summary>
		/// <param name="callback"></param>
		void SetOnCollision(const std::function<void(Collider*)>& callback) { onCollisionCallback_ = callback; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
	private:

		// ワールド座標変換
		WorldTransform worldTransform_;

		// 形状
		CollisionShape shape_;

		// 種別ID
		uint32_t typeID_ = 0u;

		// 衝突時のコールバック関数
		std::function<void(Collider*)> onCollisionCallback_ = nullptr;

		// 線描画マネージャのポインタ
		LineManager* lineManager_ = nullptr;
	};
}
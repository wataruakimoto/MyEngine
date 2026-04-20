#pragma once

#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "OBB.h"
#include "Capsule.h"

#include <list>

namespace Engine {

	/// ===== 前方宣言 ===== ///

	class Collider;

	/// <summary>
	/// 衝突管理クラス
	/// </summary>
	class CollisionManager {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
	public:

		// 初期化
		void Initialize();

		// 更新
		void Update();

		// 描画
		void Draw();

		// リセット
		void Reset();

		// コライダーの追加
		void AddCollider(Collider* collider);

		// 全ての衝突判定の確認
		void CheckAllCollisions();

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
	private:

		/// <summary>
		/// コライダー2つの衝突判定と応答
		/// </summary>
		/// <param name="colliderA">当たり判定A</param>
		/// <param name="colliderB">当たり判定B</param>
		void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

		/// <summary>
		/// 球と球の衝突判定
		/// </summary>
		/// <param name="sphereA">球A</param>
		/// <param name="sphereB">球B</param>
		/// <returns>衝突しているかどうか</returns>
		bool CheckSphereToSphere(const Sphere* sphereA, const Sphere* sphereB);

		/// <summary>
		/// 球と平面の衝突判定
		/// </summary>
		/// <param name="sphere">球</param>
		/// <param name="plane">平面</param>
		/// <returns>衝突しているかどうか</returns>
		bool CheckSphereToPlane(const Sphere* sphere, const Plane* plane);

		/// <summary>
		/// 球とAABBの衝突判定
		/// </summary>
		/// <param name="sphere">球</param>
		/// <param name="aabb">AABB</param>
		/// <returns>衝突しているかどうか</returns>
		bool CheckSphereToAABB(const Sphere* sphere, const AABB* aabb);

		/// <summary>
		/// 球とOBBの衝突判定
		/// </summary>
		/// <param name="sphere">球</param>
		/// <param name="obb">OBB</param>
		/// <returns>衝突しているかどうか</returns>
		bool CheckSphereToOBB(const Sphere* sphere, const OBB* obb);

		/// <summary>
		/// 球とカプセルの衝突判定
		/// </summary>
		/// <param name="sphere"球</param>
		/// <param name="capsule">カプセル</param>
		/// <returns>衝突しているかどうか</returns>
		bool CheckSphereToCapsule(const Sphere* sphere, const Capsule* capsule);

		/// <summary>
		/// AABBとAABBの衝突判定
		/// </summary>
		/// <param name="aabbA">AABB A</param>
		/// <param name="aabbB">AABB B</param>
		/// <returns>衝突しているかどうか</returns>
		bool CheckAABBToAABB(const AABB* aabbA, const AABB* aabbB);

		/// <summary>
		/// AABBとOBBの衝突判定
		/// </summary>
		/// <param name="aabb">AABB</param>
		/// <param name="obb">OBB</param>
		/// <returns>衝突しているかどうか</returns>
		bool CheckAABBToOBB(const AABB* aabb, const OBB* obb);

		/// <summary>
		/// AABBとカプセルの衝突判定
		/// </summary>
		/// <param name="aabb">AABB</param>
		/// <param name="capsule">カプセル</param>
		/// <returns>衝突しているかどうか</returns>
		bool CheckAABBToCapsule(const AABB* aabb, const Capsule* capsule);

		/// <summary>
		/// OBBとOBBの衝突判定
		/// </summary>
		/// <param name="obbA">OBB A</param>
		/// <param name="obbB">OBB B</param>
		/// <returns>衝突しているかどうか</returns>
		bool CheckOBBToOBB(const OBB* obbA, const OBB* obbB);

		/// <summary>
		/// OBBとカプセルの衝突判定
		/// </summary>
		/// <param name="obb">OBB</param>
		/// <param name="capsule">カプセル</param>
		/// <returns>衝突しているかどうか</returns>
		bool CheckOBBToCapsule(const OBB* obb, const Capsule* capsule);
		
		/// <summary>
		/// AABBとカプセルの衝突判定
		/// </summary>
		/// <param name="capsuleA">カプセルA</param>
		/// <param name="capsuleB">カプセルB</param>
		/// <returns>衝突しているかどうか</returns>
		bool CheckCapsuleToCapsule(const Capsule* capsuleA, const Capsule* capsuleB);

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
	private:

		// コライダーのリスト
		std::list<Collider*> colliders_;

		// コライダーを表示するかどうか
		bool isDraw = true;
	};
}

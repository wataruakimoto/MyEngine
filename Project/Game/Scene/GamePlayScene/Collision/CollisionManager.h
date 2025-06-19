#pragma once
#include <list>

class Collider;

///-------------------------------------------/// 
/// 衝突マネージャ
///-------------------------------------------///
class CollisionManager {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
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
	
///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	// コライダー2つの衝突判定と応答
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// コライダーのリスト
	std::list<Collider*> colliders_;

	// コライダーを表示するかどうか
	bool isDraw = true;
};


#pragma once

#include "Collider.h"
#include "WorldTransform.h"

#include <memory>

/// <summary>
/// キャラクターの基底クラス
/// </summary>
class BaseCharacter : public Engine::Collider {

/// ================================================== ///
/// メンバ関数
/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	/// <param name="other">衝突相手</param>
	virtual void OnCollision(Collider* other) = 0;

/// ================================================== ///
/// ゲッター
/// ================================================== ///
public:

	/// <summary>
	/// ワールド変換のゲッター
	/// </summary>
	/// <returns></returns>
	Engine::WorldTransform& GetWorldTransform() { return worldTransform_; }

	Engine::Collider* GetCollider() { return collider_.get(); }

/// ================================================== ///
/// セッター
/// ================================================== ///
public:

	/// <summary>
	/// ワールド変換のセッター
	/// </summary>
	/// <param name="worldTransform"></param>
	void SetWorldTransform(const Engine::WorldTransform& worldTransform) { worldTransform_ = worldTransform; }

/// ================================================== ///
/// メンバ変数
/// ================================================== ///
protected:

	// ワールド変換
	Engine::WorldTransform worldTransform_;

	// コライダー
	std::unique_ptr<Engine::Collider> collider_;
};

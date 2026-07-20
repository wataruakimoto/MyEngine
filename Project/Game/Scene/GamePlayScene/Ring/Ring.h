#pragma once

#include "Model/Model.h"
#include "Object/Object3d.h"

#include "Collision/BaseCharacter.h"

/// <summary>
/// 加速リングクラス
/// </summary>
class Ring : public BaseCharacter {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	/// <param name="other"></param>
	void OnCollision(Engine::Collider* other) override;

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// モデル
	std::unique_ptr<Engine::Model> model_ = nullptr;

	// オブジェクト3D
	std::unique_ptr<Engine::Object3d> object_ = nullptr;

	bool isDead_ = false;
};


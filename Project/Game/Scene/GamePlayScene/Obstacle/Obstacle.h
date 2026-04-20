#pragma once

#include "Model/Model.h"
#include "Object/Object3d.h"

#include "Collision/Basecharacter.h"

#include <memory>

/// <summary>
/// 障害物クラス
/// </summary>
class Obstacle : public BaseCharacter {

/// ================================================== ///
/// メンバ関数
/// ================================================== ///
public:

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void OnCollision(Engine::Collider* other) override;

	void ShowImGui();

/// ================================================== ///
/// メンバ変数
/// ================================================== ///
private:

	// モデル
	std::unique_ptr<Engine::Model> model_ = nullptr;

	// オブジェクト3D
	std::unique_ptr<Engine::Object3d> object_ = nullptr;
};


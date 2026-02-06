#include "Reticle.h"
#include "Input.h"
#include "Camera.h"
#include "MathVector.h"
#include "MathMatrix.h"

using namespace Engine;
using namespace MathVector;
using namespace MathMatrix;

void Reticle::Initialize() {

	/// ===== スプライトの初期化 ===== ///

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("2DReticle.png");
	sprite_->SetAnchorPoint({ 0.5f, 0.5f });

	/// ===== モデル・オブジェクトの初期化 ===== ///

	// モデルの生成・初期化
	model_ = std::make_unique<Model>();
	model_->Initialize("Reticle", "Reticle.obj");

	// オブジェクトの生成・初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize();
	object_->SetModel(model_.get());
}

void Reticle::Update() {

	// マウスの位置をスクリーン座標として取得
	screenPosition_ = Input::GetInstance()->GetMousePosition();

	// カメラからビュープロジェクション行列を受け取り、逆行列に変換
	Matrix4x4 inverseViewProjectionMatrix = Inverse(camera_->GetViewProjectionMatrix());

	// スクリーン座標をワールド座標に変換
	Vector3 worldPosition = ConvertScreenToWorld(screenPosition_, inverseViewProjectionMatrix, kDistancePlayerToReticle_);

	/// ===== スプライトの更新 ===== ///

	// スクリーン座標をスプライトに設定
	sprite_->SetPosition(screenPosition_);

	// スプライトの更新
	sprite_->Update();

	/// ===== オブジェクトの更新 ===== ///

	// ワールド座標をレティクルの座標に設定
	worldTransform_.SetTranslate(worldPosition);

	// ワールド変換の更新
	worldTransform_.UpdateMatrix();

	// オブジェクトの座標を設定
	object_->SetTranslate(worldTransform_.GetTranslate());
	// オブジェクトの更新
	object_->Update();
}

void Reticle::Draw2D() {

	// スプライトの描画
	sprite_->Draw();
}

void Reticle::Draw3D() {

	// オブジェクトの描画
	object_->Draw();
}
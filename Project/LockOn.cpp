#include "LockOn.h"
#include "Scene/GamePlayScene/Enemy/Enemy.h"
#include "Math/MathMatrix.h"

using namespace MathMatrix;

LockOn::LockOn() {

	// テクスチャ読み込み
	textureManager_->LoadTexture("resources/reticle/lockon.png");
}

void LockOn::Initialize() {

	// スプライトの生成・初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("resources/reticle/lockon.png");
	sprite_->SetAnchorPoint({ 0.5f, 0.5f }); // アンカーを中心に設定
}

void LockOn::Update() {

	// ロックオン判定処理
	for (Enemy* enemy : enemies_) {
		
		// 敵のワールド座標を取得
		Vector3 enemyPos = enemy->GetWorldTransform().GetWorldPosition();
		
		// ワールド座標からスクリーン座標に変換

	}

	// ロックオン判定の敵がいたら


	// スプライトの更新
	sprite_->Update();
}

void LockOn::Draw() {

	// スプライトの描画
	sprite_->Draw();
}

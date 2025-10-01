#include "LockOn.h"
#include "Camera/Camera.h"
#include "Scene/GamePlayScene/Enemy/Enemy.h"
#include "Scene/GamePlayScene/Reticle/Reticle2D.h"
#include "Math/MathVector.h"

using namespace MathVector;

LockOn::LockOn() {

	// テクスチャ読み込み
	textureManager_->LoadTexture("resources/LockOn.png");
}

void LockOn::Initialize() {

	// スプライトの生成・初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("resources/LockOn.png");
	sprite_->SetAnchorPoint({ 0.5f, 0.5f }); // アンカーを中心に設定
}

void LockOn::Update() {

	// ロックオン判定処理
	for (Enemy* enemy : enemies_) {

		// 敵のワールド座標を取得
		Vector3 enemyPos = enemy->GetWorldTransform().GetWorldPosition();

		// ワールド座標からスクリーン座標に変換
		Vector2 screenPos = ConvertWorldToScreen(enemyPos, camera_->GetViewProjectionMatrix());

		// 2Dレティクルの座標を取得
		Vector2 reticlePos = reticle2D_->GetReticlePosition();

		// スプライトからの距離
		float distance = Length(reticlePos - screenPos);

		// 2Dレティクルからの距離が範囲内なら
		if (distance < kDistanceLockOn_) {

			targets_.emplace_back(std::make_pair(distance, enemy));
		}
	}

	// 一旦ロックオン対象を解除
	target_ = nullptr;

	// ロックオン判定の敵がいたら
	if (!targets_.empty()) {

		// 距離で昇順ソート
		targets_.sort();

		// 距離が一番小さい敵をロックオンする
		target_ = targets_.front().second;

		// 敵のワールド座標を取得
		Vector3 targetPos = target_->GetWorldTransform().GetWorldPosition();

		// ワールド座標からスクリーン座標に変換
		Vector2 screenPos = ConvertWorldToScreen(targetPos, camera_->GetViewProjectionMatrix());

		// スプライトの座標をスクリーン座標に設定
		sprite_->SetPosition(screenPos);
	}

	// スプライトの更新
	sprite_->Update();
}

void LockOn::Draw() {

	// スプライトの描画
	sprite_->Draw();
}

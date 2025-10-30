#include "LockOn.h"
#include "Camera.h"
#include "Enemy/Enemy.h"
#include "Player/Player.h"
#include "Reticle/Reticle2D.h"
#include "MathVector.h"

#include <imgui.h>

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

	// ロックオン対象を探索
	SearchTarget();

	// ロックオン対象を決定
	DecideTarget();

	// スプライトの更新
	sprite_->Update();
}

void LockOn::Draw() {

	// ロックオン対象がいたら
	if (target_) {
		
		// スプライトの描画
		sprite_->Draw();
	}
}

void LockOn::ShowImGui() {

#ifdef _DEBUG

	ImGui::Begin("LockOn");

	// ロックオン対象を表示
	if (target_) {
		ImGui::Text("LockOn Target: %p", target_);
	}
	else {
		ImGui::Text("LockOn Target: nullptr");
	}

	// ロックオン対象リストを表示
	ImGui::Text("Targets List:");

	for (const auto& target : targets_) {
		// レティクルからの距離、自機からの距離、敵ポインタを表示
		ImGui::Text("%.2f, %.2f, %p", std::get<0>(target), std::get<1>(target), std::get<2>(target));
	}
	
	ImGui::End();
	
#endif // _DEBUG
}

void LockOn::SearchTarget() {

	// ロックオン対象リストをクリア
	targets_.clear();

	// 自機からワールド座標を取得
	Vector3 playerPos = player_->GetWorldTransform().GetWorldPosition();

	// 自機のワールド座標をビュー座標に変換
	Vector3 playerViewPos = MathVector::Transform(playerPos, camera_->GetViewMatrix());

	// ロックオン対象を探索する
	for (Enemy* enemy : enemies_) {

		// 敵が死んでいたら
		if (enemy->IsDead()) {

			// 次の敵へ
			continue;
		}

		// 敵のワールド座標を取得
		Vector3 enemyPos = enemy->GetWorldTransform().GetWorldPosition();

		// ワールド座標からビュー座標に変換
		Vector3 enemyViewPos = MathVector::Transform(enemyPos, camera_->GetViewMatrix());

		// 自機より後ろにいる敵は
		if (enemyViewPos.z <= playerViewPos.z) {

			// 対象にしない
			continue;
		}

		// ワールド座標からスクリーン座標に変換
		Vector2 screenPos = ConvertWorldToScreen(enemyPos, camera_->GetViewProjectionMatrix());

		// 2Dレティクルの座標を取得
		Vector2 reticlePos = reticle2D_->GetReticlePosition();

		// レティクルからの距離
		float reticleDistance = Length(reticlePos - screenPos);

		// 自機からの距離
		float playerDistance = Length(playerPos - enemyPos);

		// 3D空間での距離が範囲内でなければ
		if (playerDistance > kDistanceLockOn3D_) {

			// 次の敵へ
			continue;
		}

		// 2Dレティクルからの距離が範囲内なら
		if (reticleDistance < kDistanceLockOn_) {

			// ロックオン対象リストに追加
			targets_.emplace_back(std::make_tuple(reticleDistance, playerDistance, enemy));
		}
	}
}

void LockOn::DecideTarget() {

	// 一旦ロックオン対象を解除
	target_ = nullptr;

	// ロックオン中フラグを下げとく
	isLockOn_ = false;

	// ロックオン判定の敵がいたら
	if (!targets_.empty()) {

		// 2つの条件を元にソート
		targets_.sort(

			[](const std::tuple<float, float, Enemy*>& a, const std::tuple<float, float, Enemy*>& b) {

				// レティクルからの距離が同じではなかったら
				if (std::get<0>(a) != std::get<0>(b)) { // <0>はtupleの1番目の要素(レティクルからの距離)

					// レティクルからの距離が近い順にソート
					return std::get<0>(a) < std::get<0>(b);
				}
				// レティクルからの距離が同じなら
				else {

					// 自機からの距離が近い順にソート
					return std::get<1>(a) < std::get<1>(b); // <1>はtupleの2番目の要素(自機からの距離)
				}
			}
		);

		// ソートした結果、リストの一番前が最もロックオンに適した敵になる
		target_ = std::get<2>(targets_.front()); // <2>はtupleの3番目の要素(敵ポインタ)

		// 敵のワールド座標を取得
		Vector3 targetPos = target_->GetWorldTransform().GetWorldPosition();

		// ワールド座標からスクリーン座標に変換
		Vector2 screenPos = ConvertWorldToScreen(targetPos, camera_->GetViewProjectionMatrix());

		// スプライトの座標をスクリーン座標に設定
		sprite_->SetPosition(screenPos);

		// ロックオン中フラグを立てる
		isLockOn_ = true;
	}
}

#include "LockOn.h"
#include "Camera.h"
#include "Enemy/Enemy.h"
#include "Player/Player.h"
#include "Reticle/Reticle.h"
#include "MathVector.h"

#include <imgui.h>

using namespace MathVector;

void LockOn::Initialize() {

	target_ = nullptr;

	// スプライトの生成・初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("LockOn.png");
	sprite_->SetAnchorPoint({ 0.5f, 0.5f }); // アンカーを中心に設定
}

void LockOn::Update() {

	if (target_) {

		// 敵のワールド座標を取得
		Vector3 targetPos = target_->GetWorldTransform().GetWorldPosition();

		// ワールド座標からスクリーン座標に変換
		Vector2 screenPos = ConvertWorldToScreen(targetPos, camera_->GetViewProjectionMatrix());

		// スプライトの座標をスクリーン座標に設定
		sprite_->SetPosition(screenPos);
	}
	else {

		// レティクルのスクリーン座標を取得
		Vector2 reticleScreenPos = reticle_->GetScreenPosition();

		// スプライトの座標をレティクルのスクリーン座標に設定
		sprite_->SetPosition(reticleScreenPos);
	}

	// スプライトの更新
	sprite_->Update();
}

void LockOn::Draw() {

	if (target_) {

		// スプライトの描画
		sprite_->Draw();
	}
}

void LockOn::SearchTarget(const std::list<std::unique_ptr<Enemy>>& enemies) {

	// 自機からワールド座標を取得
	Vector3 playerPos = player_->GetWorldTransform().GetWorldPosition();

	// レティクルのスクリーン座標を取得
	Vector2 reticleScreenPos = reticle_->GetScreenPosition();

	/// ===== 継続判定 ===== ///

	// 維持フラグ
	bool isKeepTarget = false;

	if (target_) {

		// ターゲットが生きていたら
		if (!target_->IsDead()) {

			// ターゲットのワールド座標を取得
			Vector3 targetPos = target_->GetWorldTransform().GetWorldPosition();

			// 自機からターゲットまでの距離を計算
			float distanceToTarget = Length(targetPos - playerPos);

			// ターゲットスクリーン座標を取得
			Vector2 targetScreenPos = ConvertWorldToScreen(targetPos, camera_->GetViewProjectionMatrix());

			// レティクルからターゲットまでの距離を計算
			float distanceToReticle = Length(targetScreenPos - reticleScreenPos);

			// 距離がロックオン維持範囲内なら
			if (distanceToTarget <= kMaxDistanceZ && distanceToReticle <= kKeepRadius) {

				// まだロックオンを維持
				isKeepTarget = true;
			}
		}
	}

	// 維持判定なら終了
	if (isKeepTarget) return;

	// ターゲットをクリア
	target_ = nullptr;

	// ロックオン対象をクリア
	Enemy* newTarget = nullptr;

	// 最も近い敵を探すための比較用
	float closestDistance = FLT_MAX;

	// ロックオン対象を探索する
	for (const auto& enemy : enemies) {

		/// ===== 死んでいる敵 ===== ///

		// 死んでいる敵はスキップ
		if (enemy->IsDead()) continue;

		/// ===== 遠い敵 ===== ///

		// 敵のワールド座標を取得
		Vector3 enemyPos = enemy->GetWorldTransform().GetWorldPosition();

		// 自機から敵までの距離を計算
		float distanceToEnemy = Length(enemyPos - playerPos);

		// 遠すぎる敵はスキップ
		if (distanceToEnemy > kMaxDistanceZ) continue;

		/// ===== ロックオン範囲外の敵 ===== ///

		// 敵のワールド座標をスクリーン座標に変換
		Vector2 screenPos = ConvertWorldToScreen(enemyPos, camera_->GetViewProjectionMatrix());

		// レティクルから敵までの距離を計算
		float distanceToReticle = Length(screenPos - reticleScreenPos);

		// ロックオン範囲外の敵はスキップ
		if (distanceToReticle > kMaxRadius) continue;

		/// ===== 最も近い敵 ===== ///

		// 敵までの距離が最も近いか比較
		if (distanceToEnemy < closestDistance) {

			// 最も近い敵を更新
			closestDistance = distanceToEnemy;
			newTarget = enemy.get();
		}
	}

	// ロックオン対象を更新
	// 見つからなければ nullptr が入る
	target_ = newTarget;
}

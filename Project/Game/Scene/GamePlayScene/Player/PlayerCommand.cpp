#include "PlayerCommand.h"
#include "Player/Player.h"
#include "Reticle/Reticle.h"
#include "LockOn/LockOn.h"
#include "GamePlayScene.h"
#include "Bullet.h"
#include "MathVector.h"

using namespace MathVector;

void NormalShotCommand::Execute(const PlayerContext& context) {

	// プレイヤーの位置を取得
	Vector3 playerPos = context.player->GetWorldTransform().GetWorldPosition();

	// レティクルの位置を取得
	Vector3 reticlePos = context.reticle->GetWorldTransform().GetWorldPosition();

	// 弾の生成&初期化
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
	bullet->Initialize();

	// 弾の位初期置をプレイヤーの位置に設定
	bullet->GetWorldTransform().SetTranslate(playerPos);

	// 方向ベクトルを計算
	Vector3 direction = reticlePos - playerPos;

	// 正規化
	direction = Normalize(direction);

	// 弾の方向を設定
	bullet->SetDirection(direction);

	// ゲームプレイシーンの弾をリストに登録
	context.scene->AddPlayerBullet(std::move(bullet));
}

void LockOnAimCommand::Execute(const PlayerContext& context) {

	// ロックオンのターゲットを更新
	context.lockOn->SearchTarget(context.scene->GetEnemies());
}

void LockOnShotCommand::Execute(const PlayerContext& context) {

	// プレイヤーの位置を取得
	Vector3 playerPos = context.player->GetWorldTransform().GetWorldPosition();

	// ターゲットの位置
	Vector3 targetPos;

	// ロックオンがターゲットを持っていたら
	if (context.lockOn->GetTarget()) {

		// ターゲットの位置を取得
		targetPos = context.lockOn->GetTarget()->GetWorldTransform().GetWorldPosition();
	}
	// 持っていなければ
	else {
		
		// レティクルの位置を取得
		targetPos = context.reticle->GetWorldTransform().GetWorldPosition();
	}

	// 弾の生成&初期化
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
	bullet->Initialize();

	// 弾の位初期置をプレイヤーの位置に設定
	bullet->GetWorldTransform().SetTranslate(playerPos);

	// 方向ベクトルを計算
	Vector3 direction = targetPos - playerPos;

	// 正規化
	direction = Normalize(direction);

	// 弾の方向を設定
	bullet->SetDirection(direction);

	// ゲームプレイシーンの弾をリストに登録
	context.scene->AddPlayerBullet(std::move(bullet));
}

void BarrelRollCommand::Execute(const PlayerContext& context) {
}

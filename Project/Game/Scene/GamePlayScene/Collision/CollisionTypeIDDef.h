#pragma once
#include <cstdint>

// コリション種別ID定義
enum class CollisionTypeIDDef : uint32_t {

	kDefault, // デフォルト
	kPlayer, // プレイヤー
	kEnemy, // 敵
	kPlayerBullet, // 弾
	kEnemyBullet, // 敵の弾
};
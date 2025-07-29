#pragma once
#include <cstdint>

// コリション種別ID定義
enum class CollisionTypeIDDef : uint32_t {

	kDefault, // デフォルト
	kPlayer, // プレイヤー
	kBullet, // 弾
	kEnemy, // 敵
};
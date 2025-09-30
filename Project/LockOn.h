#pragma once

#include "2D/Sprite.h"
#include "2D/TextureManager.h"

#include <list>
#include <memory>

/// === 前方宣言 === ///
class Enemy;

/// === ロックオン === ///
class LockOn {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LockOn();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	void SetEnemies(std::list<Enemy*> enemies) { enemies_ = enemies; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 敵のリスト
	std::list<Enemy*> enemies_;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// テクスチャマネージャのインスタンス
	TextureManager* textureManager_ =  TextureManager::GetInstance();
};


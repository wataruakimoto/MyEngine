#pragma once

#include "IPlayerState.h"
#include "WorldTransform.h"
#include "Reticle/Reticle.h"

#include <memory>
#include <numbers>

/// === 前方宣言 === ///

namespace Engine {
	class Camera;
}

/// <summary>
/// マニュアル操作状態のコンテキスト
/// </summary>
struct ManualStateContext {

	Engine::WorldTransform* worldTransform = nullptr; // ワールド変換
	float* moveSpeed = nullptr;						  // 移動の速さ
	Engine::Camera* camera = nullptr;				  // カメラ
};

/// <summary>
/// マニュアル操作状態
/// </summary>
class PlayerManualState : public IPlayerState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="context">コンテキスト</param>
	void Initialize(const ManualStateContext& context);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// UIの描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 状態に入ったときの処理
	/// </summary>
	void Enter() override;

	/// <summary>
	/// 状態から出るときの処理
	/// </summary>
	void Exit() override;

	/// <summary>
	/// 被弾時の処理
	/// </summary>
	void OnHit();

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// レティクルに向かって移動
	/// </summary>
	void MoveToReticle();

	/// <summary>
	/// 移動範囲の制限
	/// </summary>
	void ClampPosition();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// コンテキスト
	ManualStateContext context_;

	// レティクル
	std::unique_ptr<Reticle> reticle_ = nullptr;

	/// ========== 移動用 ========== ///

	// 速度
	Engine::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	// 移動範囲の制限 半径
	const float kMaxRadius_ = 25.0f;

	// 地面の高さの制限
	const float kMinHeight_ = 0.0f;
};


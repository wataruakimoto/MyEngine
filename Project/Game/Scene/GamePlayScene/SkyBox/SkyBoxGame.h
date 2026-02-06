#pragma once

#include "Skybox/Skybox.h"

#include <memory>

/// === 前方宣言 === ///
class Engine::Camera;
class Player;

/// ===== 天球 ===== ///
class SkyBoxGame {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

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

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui();

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Engine::Camera* camera) { skybox->SetCamera(camera); }

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// スカイボックス
	std::unique_ptr<Engine::Skybox> skybox = nullptr;

	// プレイヤーの借りポインタ
	Player* player_ = nullptr;
};


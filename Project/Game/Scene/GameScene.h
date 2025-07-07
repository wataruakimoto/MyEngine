#pragma once

#include "BaseScene.h"

#include <memory>

class SkyBox;

/// ===== ゲームシーン ===== ///
class GameScene : public BaseScene {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// スカイボックス
	std::unique_ptr<SkyBox> skyBox = nullptr;
};
#pragma once

#include "ParticleData.h"

#include <string>

/// === 前方宣言 === ///

namespace Engine {
	class ParticleManager;
}

/// <summary>
/// パーティクルエディター
/// </summary>
class ParticleEditor {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui();

	/// <summary>
	/// JSONに設定を保存
	/// </summary>
	void SaveSettingToJSON();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// 編集中のエフェクトの名前
	std::string editingEffectName = "";
	
	// 編集中のエフェクトの設定
	Engine::ParticleSetting editingSetting = {};

	// パーティクルマネージャーのインスタンス
	Engine::ParticleManager* particleManager = nullptr;
};


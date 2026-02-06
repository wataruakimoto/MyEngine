#pragma once

#include "BaseScene.h"

#include <string>

namespace Engine {

	/// ===== 抽象シーンファクトリー ===== ///
	class AbstractSceneFactory {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~AbstractSceneFactory() = default;

		/// <summary>
		/// シーン生成
		/// </summary>
		/// <param name="sceneName">シーン名</param>
		/// <returns>BaseScene</returns>
		virtual BaseScene* CreateScene(const std::string& sceneName) = 0;

		/// <summary>
		/// ImGui表示
		/// </summary>
		virtual void ShowImGui() = 0;

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	protected:

		// 現在のシーン名
		std::string currentSceneName_;
	};
}
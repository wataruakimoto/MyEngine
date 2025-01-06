#pragma once

#include "AbstractSceneFactory.h"

/// ===== シーンファクトリー ===== ///
class SceneFactory : public AbstractSceneFactory {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>BaseScene</returns>
	BaseScene* CreateScene(const std::string& sceneName) override;
};
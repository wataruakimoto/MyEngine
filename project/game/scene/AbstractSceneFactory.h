#pragma once

#include "BaseScene.h"
#include <string>

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
};
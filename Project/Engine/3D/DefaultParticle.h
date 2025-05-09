#pragma once
#include "BaseParticleType.h"

/// === デフォルトのパーティクル === ///
class DefaultParticle : public BaseParticleType {

///-------------------------------------------///
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 頂点データ生成
	/// </summary>
	void GenerateVertexData() override;

	/// <summary>
	/// 参照データ生成
	/// </summary>
	void GenerateIndexData() override;

	/// <summary>
	/// マテリアルデータ生成
	/// </summary>
	void GenerateMaterialData() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:
};


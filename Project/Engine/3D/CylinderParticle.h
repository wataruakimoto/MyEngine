#pragma once
#include "BaseParticleType.h"

/// === シリンダ用のパーティクル === ///
class CylinderParticle : public BaseParticleType {

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

	///-------------------------------------------/// 
/// シリンダー用の変数
///-------------------------------------------///

	// シリンダーの設定
	const uint32_t kCylinderDivide = 32; // 円の分割数
	const float kTopRadius = 1.0f; // 上側の半径
	const float kBottomRadius = 1.0f; // 下側の半径
	const float kHeight = 3.0f; // 高さ
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kCylinderDivide); // 分割あたりのラジアン 2π/分割数
};


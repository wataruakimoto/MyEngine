#pragma once

#include "BaseParticleType.h"

/// ===== キューブパーティクル ===== ///
class CubeParticle : public BaseParticleType {

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
	void Draw(ParticleGroup* group) override;

	///-------------------------------------------/// 
	/// クラス内関数
	///-------------------------------------------///
private:

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

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:
};

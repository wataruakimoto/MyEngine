#pragma once
#include "Data/VertexData.h"
#include "Math/Matrix4x4.h"
#include "ParticleSystem.h"

#include <d3d12.h>
#include <stdint.h>
#include <wrl.h>

// マテリアルデータ
struct Material {
	Vector4 color;
	Matrix4x4 uvTransform;
};

/// === パーティクルタイプの基底クラス === ///
class BaseParticleType {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseParticleType() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(ParticleGroup* group) = 0;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
protected:

	/// <summary>
	/// 頂点データ生成
	/// </summary>
	virtual void GenerateVertexData() = 0;

	/// <summary>
	/// 参照データ生成
	/// </summary>
	virtual void GenerateIndexData() = 0;

	/// <summary>
	/// マテリアルデータ生成
	/// </summary>
	virtual void GenerateMaterialData() = 0;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
protected:

	// 頂点リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource;

	// 頂点データ
	VertexData* vertexData = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	// 参照リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> indexResource;

	// 参照データ
	uint32_t* indexData = nullptr;

	// Index用のビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	// マテリアルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource;

	// マテリアルデータ
	Material* materialData = nullptr;
};
#pragma once

#include "ParticleData.h"
#include "Data/VertexData.h"

#include <list>
#include <d3d12.h>
#include <wrl.h>

/// === 前方宣言 === ///
class DirectXUtility;
class TextureManager;
class SrvManager;
class Camera;

/// ===== パーティクル描画用の抽象基底クラス ===== ///
class ParticleRendererBase {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~ParticleRendererBase() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="instanceCount">インスタンス数</param>
	/// <param name="instanceSrvIndex">インスタンス用SRVインデックス</param>
	/// <param name="texturePath">テクスチャパス</param>
	virtual void Draw(uint16_t instanceCount, uint16_t instanceSrvIndex, const std::string& texturePath) = 0;

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

	// DirectXUtilityのインスタンス
	DirectXUtility* dxUtility = nullptr;

	// TextureManagerのインスタンス
	TextureManager* textureManager = nullptr;

	// SrvManagerのインスタンス
	SrvManager* srvManager = nullptr;

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
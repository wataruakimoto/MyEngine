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
	/// <param effectName="Groups">パーティクルのリスト</param>
	/// <param effectName="camera">カメラ</param>
	virtual void Draw(const std::list<ParticleInstance>& particles, const Camera& camera) = 0;

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

	/// <summary>
	/// インスタンスデータ生成
	/// </summary>
	virtual void GenerateInstanceData() = 0;

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

	// インスタンスリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> instanceResource;

	// インスタンスデータ
	InstanceData* instanceData = nullptr;

	// インスタンスの最大数
	const uint32_t kMaxInstanceCount = 1000;

	// SRVインデックス
	uint32_t srvIndex = 0;
};
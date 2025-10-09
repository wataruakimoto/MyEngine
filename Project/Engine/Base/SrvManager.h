#pragma once

#include <stdint.h>
#include <d3d12.h>
#include <wrl.h>

/// === 前方宣言 === ///
class DirectXUtility;

class SrvManager {

	///-------------------------------------------/// 
	/// シングルトン
	///-------------------------------------------///
private:

	// インスタンス
	static SrvManager* instance;

	// コンストラクタの隠蔽
	SrvManager() = default;
	// デストラクタの隠蔽
	~SrvManager() = default;
	// コピーコンストラクタの封印
	SrvManager(SrvManager&) = delete;
	// コピー代入演算子の封印
	SrvManager& operator=(SrvManager&) = delete;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static SrvManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 確保
	/// </summary>
	/// <returns></returns>
	uint32_t Allocate();

	/// <summary>
	/// 確保可能チェック
	/// </summary>
	/// <returns></returns>
	bool CheckAllocate();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// SRV生成(テクスチャ用)
	/// </summary>
	/// <param name="srvIndex"></param>
	/// <param name="pResource"></param>
	/// <param name="format"></param>
	/// <param name="mipLevels"></param>
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);

	/// <summary>
	/// SRV生成(キューブマップテクスチャ用)
	/// </summary>
	/// <param name="srvIndex"></param>
	/// <param name="pResource"></param>
	/// <param name="format"></param>
	/// <param name="mipLevels"></param>
	void CreateSRVforTextureCube(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);

	/// <summary>
	/// SRV生成(構造化バッファ用)
	/// </summary>
	/// <param name="srvIndex"></param>
	/// <param name="pResource"></param>
	/// <param name="numElements"></param>
	/// <param name="structureByteStride"></param>
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);

	/// <summary>
	/// SRV生成(レンダーテクスチャ用)
	/// </summary>
	/// <param name="srvIndex"></param>
	/// <param name="pResource"></param>
	/// <param name="format"></param>
	void CreateSRVforRenderTexture(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format);

	/// <summary>
	/// SRV生成(深度ステンシル用)
	/// </summary>
	/// <param name="srvIndex"></param>
	/// <param name="pResource"></param>
	/// <param name="format"></param>
	void CreateSRVforDepthStencil(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format);

	void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex);

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return descriptorHeap; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// DirectXUtilityのインスタンス
	DirectXUtility* dxUtility_ = nullptr;

	// SRV用デスクリプタサイズ
	uint32_t descriptorSize;

	// SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;

	// 次に使用するSRVインデックス
	uint32_t useIndex = 0;

///-------------------------------------------/// 
/// 定数
///-------------------------------------------///
public:
	// 最大SRV数(最大テクスチャ枚数)
	static const uint32_t kMaxSRVCount;
};
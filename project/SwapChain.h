#pragma once

/// === スワップチェイン === ///
class SwapChain {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後
	/// </summary>
	void PostDraw();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	void SwapChainGenerate();

	/// <summary>
	/// ビューポート矩形の初期化
	/// </summary>
	void ViewportRectInitialize();

	/// <summary>
	/// シザリング矩形の初期化
	/// </summary>
	void ScissoringRectInitialize();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// スワップチェーン
	Microsoft::WRL::ComPtrComPtr <IDXGISwapChain4> swapChain = nullptr;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// スワップチェーンから引っ張ってきたリソース
	Microsoft::WRL::ComPtrComPtr <ID3D12Resource> swapChainResources[2] = { nullptr };

	// スワップチェーンリソース
	//std::array<ComPtr<ID3D12Resource>, 2> swapChainResources;

	// ビューポート矩形
	D3D12_VIEWPORT viewportRect{};

	// シザー矩形
	D3D12_RECT scissorRect{};

	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;

	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
};


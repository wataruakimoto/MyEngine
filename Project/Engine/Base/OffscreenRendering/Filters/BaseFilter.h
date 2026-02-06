#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>
#include <cstdint>

namespace Engine {

	/// ===== 前方宣言 ===== ///
	class DirectXUtility;
	class SrvManager;
	class Camera;

	/// ===== フィルター基底クラス ===== ///
	class BaseFilter {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~BaseFilter() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// 描画設定
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// ImGui表示
		/// </summary>
		virtual void ShowImGui() = 0;

		///-------------------------------------------/// 
		/// クラス内関数
		///-------------------------------------------///
	protected:

		/// <summary>
		/// RootSignature作成
		/// </summary>
		virtual void CreateRootSignature() = 0;

		/// <summary>
		/// InputLayout作成
		/// </summary>
		virtual void CreateInputLayout() = 0;

		/// <summary>
		/// BlendState作成
		/// </summary>
		virtual void CreateBlendState() = 0;

		/// <summary>
		/// RasterizerState作成
		/// </summary>
		virtual void CreateRasterizerState() = 0;

		/// <summary>
		/// VertexShader作成
		/// </summary>
		virtual void CreateVertexShader() = 0;

		/// <summary>
		/// PixelShader作成
		/// </summary>
		virtual void CreatePixelShader() = 0;

		/// <summary>
		/// DepthStencilState作成
		/// </summary>
		virtual void CreateDepthStencilState() = 0;

		/// <summary>
		/// GraphicsPipeline作成
		/// </summary>
		virtual void CreateGraphicsPipeline() = 0;

		///-------------------------------------------/// 
		/// ゲッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// 有効化フラグのゲッター
		/// </summary>
		/// <returns></returns>
		bool GetIsActive() const { return isActive; }

		///-------------------------------------------/// 
		/// セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// SRVインデックスのセッター
		/// </summary>
		/// <param name="srvIndex"></param>
		void SetSrvIndex(uint32_t& srvIndex) { this->srvIndex = srvIndex; }

		/// <summary>
		/// 有効化フラグのセッター
		/// </summary>
		/// <param name="isActive"></param>
		void SetIsActive(bool isActive) { this->isActive = isActive; }

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	protected:

		// DirectXUtilityのインスタンス
		DirectXUtility* dxUtility = nullptr;

		// SrvManagerのインスタンス
		SrvManager* srvManager = nullptr;

		// HRESULT
		HRESULT hr;

		// RootSignature
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;

		// InputLayout
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};

		// BlendState
		D3D12_BLEND_DESC blendDesc{};

		// RasterizerState
		D3D12_RASTERIZER_DESC rasterizerDesc{};

		// VertexShader
		Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = nullptr;

		// PixelShader
		Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = nullptr;

		// DepthStencilState
		D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

		// GraphicsPipeline
		Microsoft::WRL::ComPtr <ID3D12PipelineState> graphicsPipelineState = nullptr;

		// 有効化フラグ
		bool isActive = false;

		// SRVインデックス
		uint32_t srvIndex = 0;
	};
}

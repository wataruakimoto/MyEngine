#pragma once

#include "DirectXUtility.h"

#include <d3d12.h>
#include <wrl.h>
#include <string>

namespace Engine {

	// ブレンドモード
	enum class BlendMode {
		None,       // ブレンドなし
		AlphaBlend // アルファブレンド
	};

	// カリングモード
	enum class CullMode {
		Back,  // 裏面(時計回り)を表示しない
		Front, // 表面(反時計回り)を表示しない
		None   // カリングしない(裏面も表示させる)
	};

	// デプスモード
	enum class DepthMode {
		ReadWrite, // 読み書き可能
		ReadOnly,  // 読み込みのみ
		Disabled   // 無効化
	};

	/// ===== グラフィックスパイプライン生成クラス ===== ///
	class GraphicsPipelineCreator {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		GraphicsPipelineCreator();

		/// <summary>
		/// デストラクタ
		/// </summary>
		~GraphicsPipelineCreator();

		/// <summary>
		/// パイプライン生成
		/// </summary>
		/// <param name="blendMode">ブレンドモード</param>
		/// <param name="cullMode">カリングモード</param>
		/// <param name="depthMode">デプスモード</param>
		void Create(BlendMode blendMode, CullMode cullMode, DepthMode depthMode);

		///-------------------------------------------/// 
		/// クラス内関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// BlendStateの生成
		/// </summary>
		/// <param name="blendMode">ブレンドモード</param>
		void CreateBlendState(BlendMode blendMode);

		/// <summary>
		/// RasterizerStateの生成
		/// </summary>
		/// <param name="cullMode">カリングモード</param>
		void CreateRasterizerState(CullMode cullMode);

		/// <summary>
		/// VertexShaderの生成
		/// </summary>
		void CreateVertexShader();

		/// <summary>
		/// PixelShaderの生成
		/// </summary>
		void CreatePixelShader();

		/// <summary>
		/// DepthStencilStateの生成
		/// </summary>
		/// <param name="depthMode">デプスモード</param>
		void CreateDepthStencilState(DepthMode depthMode);

		///-------------------------------------------/// 
		/// セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// RootSignatureのセッター
		/// </summary>
		/// <param name="rootSignature"></param>
		void SetRootSignature(ID3D12RootSignature* rootSignature) { this->rootSignature = rootSignature; }

		/// <summary>
		/// InputLayoutのセッター
		/// </summary>
		/// <param name="inputLayoutDesc"></param>
		void SetInputLayoutDesc(const D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc) { this->inputLayoutDesc = inputLayoutDesc; }

		/// <summary>
		/// 頂点シェーダーのファイル名のセッター
		/// </summary>
		/// <param name="fileName"></param>
		void SetVSFileName(const std::wstring& fileName) { vertexShaderFileName = fileName; }

		/// <summary>
		/// ピクセルシェーダーのファイル名のセッター
		/// </summary>
		/// <param name="fileName"></param>
		void SetPSFileName(const std::wstring& fileName) { pixelShaderFileName = fileName; }

		///-------------------------------------------/// 
		/// ゲッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// グラフィックスパイプラインステートのゲッター
		/// </summary>
		/// <returns></returns>
		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetGraphicsPipelineState() const { return graphicsPipelineState; }

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		// DirectXUtilityのインスタンス
		DirectXUtility* dxUtility_ = DirectXUtility::GetInstance();

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

		// グラフィックスパイプラインステート
		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

		// シェーダーのディレクトリパス
		const std::wstring shaderDirectoryPath = L"Resources/Shaders/";

		// 頂点シェーダーのファイル名
		std::wstring vertexShaderFileName = L"";

		// 頂点シェーダーのフルパス
		std::wstring vertexShaderFullPath = L"";

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName = L"";

		// ピクセルシェーダーのフルパス
		std::wstring pixelShaderFullPath = L"";
	};
}

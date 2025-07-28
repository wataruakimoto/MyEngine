#pragma once
#include "BaseFilter.h"

/// ===== ランダムフィルター ===== ///
class RandomFilter : public BaseFilter {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 描画設定
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui() override;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// RootSignature作成
	/// </summary>
	void CreateRootSignature() override;

	/// <summary>
	/// InputLayout作成
	/// </summary>
	void CreateInputLayout() override;

	/// <summary>
	/// BlendState作成
	/// </summary>
	void CreateBlendState() override;

	/// <summary>
	/// RasterizerState作成
	/// </summary>
	void CreateRasterizerState() override;

	/// <summary>
	/// VertexShader作成
	/// </summary>
	void CreateVertexShader() override;

	/// <summary>
	/// PixelShader作成
	/// </summary>
	void CreatePixelShader() override;

	/// <summary>
	/// DepthStencilState作成
	/// </summary>
	void CreateDepthStencilState() override;

	/// <summary>
	/// GraphicsPipeline作成
	/// </summary>
	void CreateGraphicsPipeline() override;

	/// <summary>
	/// コンフィグデータ作成
	/// </summary>
	void CreateConfigData();

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	struct Config {

		float time; // 時間
	};

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// コンフィグデータ
	Config* configData = nullptr;

	// コンフィグのリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> configResource = nullptr;
};


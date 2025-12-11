#pragma once

#include "BaseFilter.h"
#include "Vector3.h"

/// ===== ビネットフィルター ===== ///
class VignetteFilter : public BaseFilter {

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
	/// コンフィグデータの生成
	/// </summary>
	void CreateConfigData();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	void SetColor(const Vector3& color) { configData->color = color; }

	void SetIntensity(float intensity) { configData->intensity = intensity; }

	void SetScale(float scale) { configData->scale = scale; }

	void SetRange(float range) { configData->range = range; }

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	struct Config {

		Vector3 color; // 色
		float intensity; // 強さ
		float scale; // スケール
		float range; // 範囲
	};

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// コンフィグデータ
	Config* configData = nullptr;

	// コンフィグ用リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> configResource = nullptr;
};


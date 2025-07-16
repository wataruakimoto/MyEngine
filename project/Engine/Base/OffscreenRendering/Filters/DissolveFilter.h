#pragma once

#include "BaseFilter.h"
#include "Math/Vector3.h"

#include <string>

/// ===== ディゾルブフィルター ===== ///
class DissolveFilter : public BaseFilter {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXUtility* dxUtility, PostEffect* postEffect) override;

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
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// maskテクスチャのファイルパスを設定する
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName"></param>
	void SetMaskTextureFilePath(std::string directoryPath, std::string fileName);

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	struct Config {

		float threshold;	  // 閾値
		Vector3 edgeColor;    // エッジの色
		float detectionRange; // 検出範囲
		float padding[3];      // パディング
	};

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// maskテクスチャのファイルパス
	std::string maskTextureFilePath = "";

	// マスクテクスチャのSRVインデックス
	uint32_t maskTextureSrvIndex = 0;

	// コンフィグデータ
	Config* configData = nullptr;
	// コンフィグのリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> configResource = nullptr;
};


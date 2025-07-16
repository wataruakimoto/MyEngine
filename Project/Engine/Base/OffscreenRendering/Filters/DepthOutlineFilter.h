#pragma once

#include "BaseFilter.h"
#include "Math/Matrix4x4.h"

/// ===== 深度アウトラインフィルター ===== ///
class DepthOutlineFilter : public BaseFilter {

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
	/// マテリアルデータ生成
	/// </summary>
	void GenerateMaterialData();

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { this->camera = camera; }

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	struct Material {

		Matrix4x4 projectionInverse; // 投影逆行列
	};

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
	// マテリアルデータ
	Material* materialData = nullptr;

	// カメラの借りポインタ
	Camera* camera = nullptr;
};


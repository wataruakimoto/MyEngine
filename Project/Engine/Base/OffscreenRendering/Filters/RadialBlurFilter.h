#pragma once

#include "BaseFilter.h"
#include "Vector3.h"
#include "Vector2.h"

namespace Engine {

	/// ===== ラジアルブラーフィルター ===== ///
	class RadialBlurFilter : public BaseFilter {

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
		/// コンフィグデータ生成
		/// </summary>
		void CreateConfigData();

		///-------------------------------------------/// 
		/// ゲッター
		///-------------------------------------------///
	public:

		float GetBlurStrength() { return configData->blurStrength; }

		///-------------------------------------------/// 
		/// セッター
		///-------------------------------------------///
	public:

		void SetGlowColor(const Vector3& glowColor) { configData->glowColor = glowColor; }

		void SetBlurStrength(float blurStrength) { configData->blurStrength = blurStrength; }

		void SetCenter(const Vector2& center) { configData->center = center; }

		///-------------------------------------------/// 
		/// 構造体
		///-------------------------------------------///
	public:

		struct Config {

			Vector3 glowColor;  // 発光色
			float blurStrength; // ぼかし強度
			Vector2 center;	    // 中心点
		};

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		// コンフィグデータ
		Config* configData = nullptr;

		// コンフィグデータ用リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> configResource = nullptr;
	};
}

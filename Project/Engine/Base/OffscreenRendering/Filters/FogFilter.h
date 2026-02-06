#pragma once

#include "BaseFilter.h"
#include "Vector4.h"

namespace Engine {

	/// ===== フォグフィルター ===== ///
	class FogFilter : public BaseFilter {

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
		/// クラス内関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// 距離を深度に変換する
		/// </summary>
		/// <param name="distance">距離</param>
		/// <returns>深度</returns>
		float ConvertDistanceToDepth(float distance);

		///-------------------------------------------/// 
		/// セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// 深度用SRVインデックスのセッター
		/// </summary>
		/// <param name="depthSrvIndex">深度用Srvインデックス</param>
		void SetDepthSrvIndex(uint32_t depthSrvIndex) { this->depthSrvIndex = depthSrvIndex; }

		void SetCamera(Camera* camera) { this->camera = camera; }

		void SetColor(const Vector4& color) { configData->color = color; }

		void SetStartDistance(float startDistance) { this->startDistance = startDistance; }

		///-------------------------------------------/// 
		/// 構造体
		///-------------------------------------------///
	public:

		struct Config {

			Vector4 color; // 発光色
			float start;   // フォグ開始距離
			float end;	   // フォグ終了距離
			float padding[2];
		};

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		// コンフィグデータ
		Config* configData = nullptr;

		// コンフィグデータ用リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> configResource = nullptr;

		// 深度用SRVインデックス
		uint32_t depthSrvIndex = 0;

		// 開始距離のデフォルト値
		float startDistance = 50.0f;

		float nearClip = 0.1f;
		float farClip = 1000.0f;

		// カメラの借りポインタ
		Camera* camera = nullptr;
	};
}

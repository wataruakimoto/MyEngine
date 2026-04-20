#pragma once

#include "BaseFilter.h"
#include "Vector3.h"

namespace Engine {

	/// <summary>
	/// ビネットフィルター
	/// </summary>
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

		void CreateGraphicsPipeline() override;

		/// <summary>
		/// コンフィグデータの生成
		/// </summary>
		void CreateConfigData();

	///-------------------------------------------/// 
	/// セッター
	///-------------------------------------------///
	public:

		void SetColor(const Vector4& color) { configData_->color = color; }

		void SetIntensity(float intensity) { configData_->intensity = intensity; }

		void SetScale(float scale) { configData_->scale = scale; }

		void SetRange(float range) { configData_->range = range; }

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
	private:

		/// <summary>
		/// コンフィグデータ
		/// </summary>
		struct ConfigData {

			Vector4 color; // 色
			float intensity; // 強さ
			float scale; // スケール
			float range; // 範囲
		};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName_ = L"Filter/Vignette.PS.hlsl";

		// コンフィグ用のバッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> configBuffer_ = nullptr;

		// コンフィグデータ
		ConfigData* configData_ = nullptr;
	};
}

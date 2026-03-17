#pragma once

#include "BaseFilter.h"
#include "Vector3.h"
#include "Vector2.h"

namespace Engine {

	/// <summary>
	/// ラジアルブラーフィルター
	/// </summary>
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
		/// グラフィックスパイプラインの生成
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

		float GetBlurStrength() { return configData_->blurStrength; }

	///-------------------------------------------/// 
	/// セッター
	///-------------------------------------------///
	public:

		void SetGlowColor(const Vector3& glowColor) { configData_->glowColor = glowColor; }

		void SetBlurStrength(float blurStrength) { configData_->blurStrength = blurStrength; }

		void SetCenter(const Vector2& center) { configData_->center = center; }

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
	private:

		/// <summary>
		/// コンフィグデータ
		/// </summary>
		struct ConfigData {

			Vector3 glowColor;  // 発光色
			float blurStrength; // ぼかし強度
			Vector2 center;	    // 中心点
		};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName_ = L"Filter/RadialBlur.PS.hlsl";

		// コンフィグ用のバッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> configBuffer_ = nullptr;

		// コンフィグデータ
		ConfigData* configData_ = nullptr;
	};
}

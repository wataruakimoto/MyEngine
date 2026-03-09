#pragma once

#include "BaseFilter.h"

namespace Engine {

	/// <summary>
	/// フェードフィルター
	/// </summary>
	class FadeFilter : public BaseFilter {

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
	/// 構造体
	///-------------------------------------------///
	private:

		/// <summary>
		/// コンフィグデータ
		/// </summary>
		struct ConfigData {

			Vector4 color; // フェード色
		};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName_ = L"Filter/Fade.PS.hlsl";

		// コンフィグ用のバッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> configBuffer_ = nullptr;

		// コンフィグデータ
		ConfigData* configData_ = nullptr;
	};
}


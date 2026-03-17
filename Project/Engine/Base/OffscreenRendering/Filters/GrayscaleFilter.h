#pragma once

#include "BaseFilter.h"

namespace Engine {

	/// <summary>
	/// グレースケールフィルター
	/// </summary>
	class GrayscaleFilter : public BaseFilter {

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

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName_ = L"Filter/Grayscale.PS.hlsl";
	};
}

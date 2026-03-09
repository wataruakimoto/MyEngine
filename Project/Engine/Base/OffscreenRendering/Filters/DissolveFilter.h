#pragma once

#include "BaseFilter.h"
#include "Vector3.h"

#include <string>

namespace Engine {

	/// <summary>
	/// ディゾルブフィルター
	/// </summary>
	class DissolveFilter : public BaseFilter {

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
		/// コンフィグデータ作成
		/// </summary>
		void CreateConfigData();

	///-------------------------------------------/// 
	/// セッター
	///-------------------------------------------///
	public:

		/// <summary>
		/// maskテクスチャの設定
		/// </summary>
		/// <param name="relativePath">相対パス</param>
		void SetMaskTextureFilePath(const std::string relativePath);

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
	private:

		struct ConfigData {

			float threshold;	  // 閾値
			Vector3 edgeColor;    // エッジの色
			float detectionRange; // 検出範囲
			float padding[3];      // パディング
		};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName_ = L"Filter/Dissolve.PS.hlsl";

		// maskテクスチャのフルパス
		std::string maskTextureFullPath = "";

		// マスクテクスチャのSRVインデックス
		uint32_t maskTextureSrvIndex = 0;

		// コンフィグ用のバッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> configBuffer_ = nullptr;

		// コンフィグデータ
		ConfigData* configData_ = nullptr;
	};
}

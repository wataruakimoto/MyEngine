#pragma once

#include "BaseFilter.h"
#include "Matrix4x4.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///

	class Camera;

	/// <summary>
	/// 深度アウトラインフィルター
	/// </summary>
	class DepthOutlineFilter : public BaseFilter {

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
	/// セッター
	///-------------------------------------------///
	public:

		/// <summary>
		/// 深度用SRVインデックスのセッター
		/// </summary>
		/// <param name="index"></param>
		void SetDepthSrvIndex(uint32_t index) { depthSrvIndex = index; }

		/// <summary>
		/// カメラのセッター
		/// </summary>
		/// <param name="camera"></param>
		void SetCamera(Camera* camera) { this->camera = camera; }

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
	private:

		/// <summary>
		/// コンフィグデータ
		/// </summary>
		struct ConfigData {

			Matrix4x4 projectionInverse; // 投影逆行列
		};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName_ = L"Filter/DepthOutline.PS.hlsl";

		// 深度用SRVインデックス
		uint32_t depthSrvIndex = 0;

		// コンフィグ用のバッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> configBuffer_ = nullptr;

		// コンフィグデータ
		ConfigData* configData_ = nullptr;

		// カメラの借りポインタ
		Camera* camera = nullptr;
	};
}

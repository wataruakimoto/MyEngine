#pragma once

#include "BaseFilter.h"
#include "Vector4.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///
	class Camera;

	/// <summary>
	/// フォグフィルター
	/// </summary>
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
		/// グラフィックスパイプラインの生成
		/// </summary>
		void CreateGraphicsPipeline() override;

		/// <summary>
		/// コンフィグデータ生成
		/// </summary>
		void CreateConfigData();

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

		void SetCamera(Camera* camera) { this->camera_ = camera; }

		void SetColor(const Vector4& color) { configData_->color = color; }

		void SetStartDistance(float startDistance) { this->startDistance = startDistance; }

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
	public:

		/// <summary>
		/// コンフィグデータ
		/// </summary>
		struct ConfigData {

			Vector4 color; // 発光色
			float start;   // フォグ開始距離
			float end;	   // フォグ終了距離
			float padding[2];
		};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// 深度用SRVインデックス
		uint32_t depthSrvIndex = 0;

		// 開始距離のデフォルト値
		float startDistance = 50.0f;

		float nearClip = 0.1f;
		float farClip = 1000.0f;

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName_ = L"Filter/Fog.PS.hlsl";

		// コンフィグ用のバッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> configBuffer_ = nullptr;

		// コンフィグデータ
		ConfigData* configData_ = nullptr;

		// カメラの借りポインタ
		Camera* camera_ = nullptr;
	};
}

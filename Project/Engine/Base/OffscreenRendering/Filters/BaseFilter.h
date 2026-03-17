#pragma once

#include "GraphicsPipelineBuilder.h"

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>
#include <cstdint>

namespace Engine {

	/// ===== 前方宣言 ===== ///
	class DirectXUtility;
	class SrvManager;

	/// ===== フィルター基底クラス ===== ///
	class BaseFilter {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
	public:

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~BaseFilter() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// 描画設定
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// ImGui表示
		/// </summary>
		virtual void ShowImGui() = 0;

	///-------------------------------------------/// 
	/// クラス内関数
	///-------------------------------------------///
	protected:

		/// <summary>
		/// グラフィックスパイプラインの生成
		/// </summary>
		virtual void CreateGraphicsPipeline() = 0;

	///-------------------------------------------/// 
	/// ゲッター
	///-------------------------------------------///
	public:

		/// <summary>
		/// 有効化フラグのゲッター
		/// </summary>
		/// <returns></returns>
		bool GetIsActive() const { return isActive_; }

	///-------------------------------------------/// 
	/// セッター
	///-------------------------------------------///
	public:

		/// <summary>
		/// SRVインデックスのセッター
		/// </summary>
		/// <param name="srvIndex"></param>
		void SetSrvIndex(uint32_t& srvIndex) { this->srvIndex_ = srvIndex; }

		/// <summary>
		/// 有効化フラグのセッター
		/// </summary>
		/// <param name="isActive"></param>
		void SetIsActive(bool isActive) { this->isActive_ = isActive; }

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	protected:

		// パイプラインビルダー
		GraphicsPipelineBuilder pipelineBuilder_;

		// 頂点シェーダーのファイル名
		std::wstring vertexShaderFileName_ = L"Filter/FullScreen.VS.hlsl";

		// 有効化フラグ
		bool isActive_ = false;

		// SRVインデックス
		uint32_t srvIndex_ = 0;

		// DirectXUtilityのインスタンス
		DirectXUtility* dxUtility_ = nullptr;

		// SrvManagerのインスタンス
		SrvManager* srvManager_ = nullptr;
	};
}

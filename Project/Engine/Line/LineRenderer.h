#pragma once

#include "GraphicsPipelineCreator.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///
	class DirectXUtility;
	class SrvManager;

	/// <summary>
	/// 線描画のレンダラー
	/// </summary>
	class LineRenderer {

	///-------------------------------------------/// 
	/// シングルトン
	///-------------------------------------------///
	private:

		// インスタンス
		static LineRenderer* instance_;

		// コンストラクタ隠蔽
		LineRenderer() = default;
		// デストラクタ隠蔽
		~LineRenderer() = default;
		// コピーコンストラクタ禁止
		LineRenderer(LineRenderer&) = delete;
		// コピーオペレータ禁止
		LineRenderer& operator=(LineRenderer&) = delete;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 描画設定
		/// </summary>
		void SettingDrawing();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

	///-------------------------------------------/// 
	/// クラス内関数
	///-------------------------------------------///
	private:

		/// <summary>
		/// ルートシグネチャ作成
		/// </summary>
		void CreateRootSignature();

		/// <summary>
		/// インプットレイアウト作成
		/// </summary>
		void CreateInputLayout();

	///-------------------------------------------/// 
	/// ゲッター
	///-------------------------------------------///
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns>インスタンス</returns>
		static LineRenderer* GetInstance();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// パイプラインクリエイター
		GraphicsPipelineCreator pipelineCreator_;

		// ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

		// インプットレイアウト	
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};

		// DirectXユーティリティのインスタンス
		DirectXUtility* dxUtility_ = nullptr;

		// Srvマネージャのインスタンス
		SrvManager* srvManager_ = nullptr;
	};
}


#pragma once

#include "GraphicsPipelineCreator.h"

// スプライト共通部
class SpriteCommon {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static SpriteCommon* instance;

	// コンストラクタの隠蔽
	SpriteCommon() = default;
	// デストラクタの隠蔽
	~SpriteCommon() = default;
	// コピーコンストラクタの封印
	SpriteCommon(SpriteCommon&) = delete;
	// コピー代入演算子の封印
	SpriteCommon& operator=(SpriteCommon&) = delete;

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static SpriteCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 共通描画設定
	/// </summary>
	void SettingDrawing();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// RootSignatureの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// InputLayoutの生成
	/// </summary>
	void CreateInputLayout();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:
	
	// DirectXUtilityのインスタンス
	DirectXUtility* dxUtility_ = DirectXUtility::GetInstance();

	// パイプラインクリエイター
	GraphicsPipelineCreator pipelineCreater_;

	// RootSignature
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
};
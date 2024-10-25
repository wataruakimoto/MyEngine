#pragma once

#include "DirectXCommon.h"

// スプライト共通部
class SpriteCommon {

///=====================================================/// 
/// メンバ関数
///=====================================================///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// 共通描画設定
	/// </summary>
	void SettingCommonDrawing();

///=====================================================/// 
/// クラス内関数
///=====================================================///
private:

	/// <summary>
	/// ルートシグネチャの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// グラフィックスパイプラインの生成
	/// </summary>
	void CreateGraphicsPipeline();

///=====================================================/// 
/// ゲッター
///=====================================================///
public:

	/// <summary>
	/// DirectXCommonのゲッター
	/// </summary>
	/// <returns></returns>
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

///=====================================================/// 
/// メンバ変数
///=====================================================///
private:
	
	// DirectXCommonのポインタ
	DirectXCommon* dxCommon_;

	HRESULT hr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr <ID3D12RootSignature> rootSignature = nullptr;
};
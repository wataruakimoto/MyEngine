#pragma once

/// === DirectX基盤 === ///
class DirectXCommon;

/// === 3Dモデル共通部=== ///
class ModelCommon {

///=====================================================/// 
/// メンバ関数
///=====================================================///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon">DirectX基盤</param>
	void Initialize(DirectXCommon* dxCommon);

///=====================================================/// 
/// ゲッター
///=====================================================///
public:

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

///=====================================================/// 
/// メンバ変数
///=====================================================///
private:

	// DirectX基盤のポインタ
	DirectXCommon* dxCommon_ = nullptr;
};
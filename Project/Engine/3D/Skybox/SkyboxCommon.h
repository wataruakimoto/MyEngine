#pragma once

#include "GraphicsPipelineCreater.h"

/// === 前方宣言 === ///
class Camera;

/// === スカイボックス基盤 === ///
class SkyboxCommon {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static SkyboxCommon* instance;
	// コンストラクタの隠蔽
	SkyboxCommon() = default;
	// デストラクタの隠蔽
	~SkyboxCommon() = default;
	// コピーコンストラクタの封印
	SkyboxCommon(SkyboxCommon&) = delete;
	// コピー代入演算子の封印
	SkyboxCommon& operator=(SkyboxCommon&) = delete;

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
	/// RootSignatureの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// InputLayoutの生成
	/// </summary>
	void CreateInputLayout();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static SkyboxCommon* GetInstance();

	/// <summary>
	/// デフォルトカメラのゲッター
	/// </summary>
	/// <returns>Camera</returns>
	Camera* GetDefaultCamera() const { return defaultCamera_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// デフォルトカメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetDefaultCamera(Camera* camera) { this->defaultCamera_ = camera; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// DirectXUtilityのインスタンス
	DirectXUtility* dxUtility_ = DirectXUtility::GetInstance();

	// パイプラインクリエイター
	GraphicsPipelineCreater pipelineCreater_;

	// RootSignature
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};

	// デフォルトカメラ
	Camera* defaultCamera_ = nullptr;
};


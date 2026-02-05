#pragma once

#include "GraphicsPipelineCreator.h"

/// === 前方宣言 === ///
class Camera;

/// === 3Dオブジェクト共通部 === ///
class Object3dCommon {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static Object3dCommon* instance;

	// コンストラクタの隠蔽
	Object3dCommon() = default;
	// デストラクタの隠蔽
	~Object3dCommon() = default;
	// コピーコンストラクタの封印
	Object3dCommon(Object3dCommon&) = delete;
	// コピー代入演算子の封印
	Object3dCommon& operator=(Object3dCommon&) = delete;

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Object3dCommon* GetInstance();

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
	void SettingDrawingOpaque();

	/// <summary>
	/// 半透明オブジェクト用描画設定
	/// </summary>
	void SettingDrawingAlpha();

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
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// デフォルトカメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetDefaultCamera(Camera* camera) { this->defaultCamera_ = camera; }

///-------------------------------------------///
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// デフォルトカメラのゲッター
	/// </summary>
	/// <returns>Camera</returns>
	Camera* GetDefaultCamera() const { return defaultCamera_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// DirectXUtilityのインスタンス
	DirectXUtility* dxUtility_ = DirectXUtility::GetInstance();

	// パイプラインクリエイター
	GraphicsPipelineCreator pipelineCreaterOpaque_;

	// 半透明用パイプラインクリエイター
	GraphicsPipelineCreator pipelineCreaterAlpha_;

	// RootSignature
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};

	// デフォルトカメラ
	Camera* defaultCamera_ = nullptr;
};
#pragma once

/// === DirectX基盤 === ///
class DirectXCommon;

/// === 3Dモデル共通部=== ///
class ModelCommon {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static ModelCommon* instance;

	// コンストラクタの隠蔽
	ModelCommon() = default;
	// デストラクタの隠蔽
	~ModelCommon() = default;
	// コピーコンストラクタの封印
	ModelCommon(ModelCommon&) = delete;
	// コピー代入演算子の封印
	ModelCommon& operator=(ModelCommon&) = delete;

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static ModelCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon">DirectX基盤</param>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// DirectX基盤のポインタ
	DirectXCommon* dxCommon_ = nullptr;
};
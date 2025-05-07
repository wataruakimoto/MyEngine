#pragma once

/// === 前方宣言 === ///
class DirectXUtility;

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
	void Initialize(DirectXUtility* dxUtility);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// DirectX基盤のポインタを取得
	/// </summary>
	/// <returns></returns>
	DirectXUtility* GetdxUtility() const { return dxUtility_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// DirectX基盤のポインタ
	DirectXUtility* dxUtility_ = nullptr;
};
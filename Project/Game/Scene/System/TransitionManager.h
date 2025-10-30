#pragma once

/// === 遷移マネージャ === ///
class TransitionManager {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static TransitionManager* instance;

	// コンストラクタ
	TransitionManager() = default;
	// デストラクタ
	~TransitionManager() = default;
	// コピーコンストラクタ削除
	TransitionManager(const TransitionManager&) = delete;
	// 代入演算子削除
	TransitionManager& operator=(const TransitionManager&) = delete;

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static TransitionManager* GetInstance();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

};


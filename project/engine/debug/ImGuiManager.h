#pragma once

/// === 前方宣言 === ///
class WinApp;
class DirectXUtility;
class SwapChain;

/// === ImGui管理クラス === ///
class ImGuiManager {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static ImGuiManager* instance;

	// コンストラクタ(隠蔽)
	ImGuiManager() = default;

	// デストラクタ(隠蔽)
	~ImGuiManager() = default;

	// コピーコンストラクタ(封印)
	ImGuiManager(ImGuiManager&) = delete;

	// コピー代入演算子(封印)
	ImGuiManager& operator=(ImGuiManager&) = delete;

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp, DirectXUtility* dxUtility, SwapChain* swapChain);

	/// <summary>
	/// 受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// 受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static ImGuiManager* GetInstance();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	SwapChain* swapChain_ = nullptr;
};
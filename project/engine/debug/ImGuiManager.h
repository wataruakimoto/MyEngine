#pragma once
#include "base/DirectXCommon.h"

/// === WindowsAPIクラス === ///
class WinApp;

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
	/// <param name="winApp">WinAppクラスのポインタ</param>
	/// <param name="dxCommon">DirectXCommonクラスのポインタ</param>
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

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

	DirectXCommon* dxCommon_ = nullptr;
};
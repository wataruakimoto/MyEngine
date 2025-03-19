#pragma once
#include "base/DirectXCommon.h"

/// === WindowsAPIクラス === ///
class WinApp;

/// === ImGui管理クラス === ///
class ImGuiManager {

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
/// メンバ変数
///-------------------------------------------///
private:

	DirectXCommon* dxCommon_ = nullptr;
};
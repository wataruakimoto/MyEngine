#pragma once

// WindowsAPIを使うため
#include <Windows.h>
// int32_tを使うため
#include <cstdint>

// WindowsAPI
class WinApp {

public: // 静的メンバ関数
	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>アプリ終了メッセージの有無</returns>
	bool ProcessMessage();

	/// <summary>
	/// HWNDのGetter
	/// </summary>
	/// <returns>hwnd</returns>
	HWND GetHwnd() const { return hwnd; }

	/// <summary>
	/// HINSTANCEのGetter
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetHInstance() const { return wc.hInstance; }

public: // 定数
	// クライアントの領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

private: // メンバ変数
	// ウィンドウハンドル
	HWND hwnd = nullptr;
	// ウィンドウクラスの設定
	WNDCLASS wc{};
};
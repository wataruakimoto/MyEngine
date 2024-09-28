#pragma once

// hInstance,hwndを使うため
#include <Windows.h>

// Comptrを使うため
#include <wrl.h>

// DirectInputの定義
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>

// 入力
class Input {

public: 

	// namespace省略
	// エイリアステンプレート
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(HINSTANCE hInstance,HWND hwnd);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private: // メンバ変数

	// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;
};
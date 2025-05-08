#pragma once

// hInstance,hwndを使うため
#include <Windows.h>
// Comptrを使うため
#include <wrl.h>
// DirectInputの定義
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
// WindowsAPIを使うため
#include "winApp/WinApp.h"

// 入力
class Input {

public:
	// namespace省略 エイリアステンプレート
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static Input* instance;
	// コンストラクタの隠蔽
	Input() = default;
	// デストラクタの隠蔽
	~Input() = default;
	// コピーコンストラクタの封印
	Input(Input&) = delete;
	// コピー代入演算子の封印
	Input& operator=(Input&) = delete;

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>トリガーしているか</returns>
	bool TriggerKey(BYTE keyNumber);

private: // メンバ変数
	// DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput;
	// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;
	// 全キーの状態
	BYTE key[256] = {};
	// 前のフレームの全キーの状態
	BYTE keyPre[256] = {};
	// WindowsAPI
	WinApp* winApp_ = nullptr;
};
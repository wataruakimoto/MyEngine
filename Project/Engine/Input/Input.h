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

#include <Xinput.h>

#include "math/Vector2.h"

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
	/// ImGuiの表示
	/// </summary>
	void ShowImgui();

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

	/// <summary>
	/// マウスカーソルの位置を取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetMousePosition();

	/// <summary>
	/// コントローラーの状態を取得
	/// </summary>
	/// <returns></returns>
	XINPUT_STATE GetControllerState();

	/// <summary>
	/// コントローラーの押下をチェック
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool PushButton(WORD button);

	/// <summary>
	/// コントローラーのトリガーをチェック
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool TriggerButton(WORD button);

	/// <summary>
	/// 右スティックがデッドゾーンを超えているか
	/// </summary>
	/// <returns></returns>
	bool IsRightThumbStickOutDeadZone();

	/// <summary>
	/// 左スティックがデッドゾーンを超えているか
	/// </summary>
	/// <returns></returns>
	bool IsLeftThumbStickOutDeadZone();

	/// <summary>
	/// 右スティックの値を取得
	/// </summary>
	Vector2 GetRightThumbStick();

	/// <summary>
	/// 左スティックの値を取得
	/// </summary>
	Vector2 GetLeftThumbStick();

	/// <summary>
	/// 右スティックのデッドゾーンの値の設定
	/// </summary>
	/// <param name="deadZone"></param>
	/// <returns></returns>
	void SetDeadZoneR(float deadZone);

	/// <summary>
	/// 左スティックのデッドゾーンの値の設定
	/// </summary>
	/// <param name="deadZone"></param>
	/// <returns></returns>
	void SetDeadZoneL(float deadZone);

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:
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

	// 現在のコントローラーの状態
	XINPUT_STATE controllerStateCurrent;
	// 前のフレームのコントローラーの状態
	XINPUT_STATE controllerStatePre;

	// デッドゾーンの値
	float deadZoneR = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE; // 右の値
	float deadZoneL = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE; // 左の値
};
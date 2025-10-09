#include "Input.h"
// assertを使うため
#include <cassert>
#include <imgui.h>

// .libファイルのリンクをする
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xinput.lib")

Input* Input::instance = nullptr;

Input* Input::GetInstance() {

	if (instance == nullptr) {
		instance = new Input;
	}
	return instance;
}

void Input::Initialize(WinApp* winApp) {

	// 借りてきたWinAppのインスタンスを記録
	this->winApp_ = winApp;

	// WinMainのresult変数を用意
	// メンバ変数 = 仮引数
	HRESULT result;

	// hInstance : Windowsのウィンドウを作成するのに必要な「インスタンスハンドル」(実態はポインタ)
	// hwnd : Windowsのウィンドウをコントロールするのに必要な「ウィンドウハンドル」(実態はポインタ)
	
	// 元データを参照して使う
	result = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	// キーボードデバイス生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	// コントローラー状態の初期化
	ZeroMemory(&controllerStateCurrent, sizeof(XINPUT_STATE));
	ZeroMemory(&controllerStatePre, sizeof(XINPUT_STATE));
}

void Input::Update(){

	// 前回キーの入力を保存
	memcpy(keyPre, key, sizeof(key));

	// キーボード情報の取得開始
	keyboard->Acquire();
	// 全キーの入力情報を取得する
	keyboard->GetDeviceState(sizeof(key), key);

	// コントローラーの状態更新
	controllerStatePre = controllerStateCurrent; // 前フレームの状態を保存
	ZeroMemory(&controllerStateCurrent, sizeof(XINPUT_STATE));
	DWORD result = XInputGetState(0, &controllerStateCurrent); // コントローラー0を取得
	if (result != ERROR_SUCCESS) {
		// コントローラーが接続されていない場合
		ZeroMemory(&controllerStateCurrent, sizeof(XINPUT_STATE));
	}
}

void Input::Finalize() {

	delete instance;
	instance = nullptr;
}

void Input::ShowImgui() {

#ifdef _DEBUG // _DEBUG

	ImGui::Begin("Input");

	// スティックの値を表示
	Vector2 rValue = Vector2(static_cast<float>(controllerStateCurrent.Gamepad.sThumbRX) / 32767.0f, static_cast<float>(controllerStateCurrent.Gamepad.sThumbRY) / 32767.0f);
	Vector2 lValue = Vector2(static_cast<float>(controllerStateCurrent.Gamepad.sThumbLX) / 32767.0f, static_cast<float>(controllerStateCurrent.Gamepad.sThumbLY) / 32767.0f);
	ImGui::SliderFloat2("Right Stick", &rValue.x, -1.0f, 1.0f);
	ImGui::SliderFloat2("Left Stick", &lValue.x, -1.0f, 1.0f);

	// デッドゾーンの値を表示
	ImGui::SliderFloat("DeadZoneR", &deadZoneR, 0.0f, 32767.0f);
	ImGui::SliderFloat("DeadZoneL", &deadZoneL, 0.0f, 32767.0f);

	ImGui::End();

#endif // _DEBUG
}

bool Input::PushKey(BYTE keyNumber) {

	// 指定したキーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	// そうでなければfalse
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 指定したキーが前フレームは押していなくて今フレームが押していればtrue
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}
	// そうでなければfalse
	return false;
}

Vector2 Input::GetMousePosition() {

	POINT mousePosition;

	// カーソル位置をスクリーン座標で取得
	if (GetCursorPos(&mousePosition)) {

		// スクリーン座標をクライアント座標に変換
		if (ScreenToClient(winApp_->GetHwnd(), &mousePosition)) {

			return Vector2(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
		}
	}
	// 取得失敗時は画面中心
	return Vector2(640.0f, 360.0f);
}

XINPUT_STATE Input::GetControllerState() {

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(0, &state); // ゲームパッドの状態を取得
	return state;
}

bool Input::PushButton(WORD button) {

	return true;
}

bool Input::TriggerButton(WORD button) {
	return true;
}

bool Input::IsRightThumbStickOutDeadZone() {

	// コントローラーの右スティックの値を取得
	short x = controllerStateCurrent.Gamepad.sThumbRX;
	short y = controllerStateCurrent.Gamepad.sThumbRY;

	// スティックの値がデッドゾーンの値に収まっているか上下左右それぞれでチェック
	if (abs(x) > deadZoneR || abs(x) < -deadZoneR) {

		return true; // デッドゾーン外
	}
	if (abs(y) > deadZoneR || abs(y) < -deadZoneR) {

		return true; // デッドゾーン外
	}
	return false; // デッドゾーン内
}

bool Input::IsLeftThumbStickOutDeadZone() {

	// コントローラーの左スティックの値を取得
	short x = controllerStateCurrent.Gamepad.sThumbLX;
	short y = controllerStateCurrent.Gamepad.sThumbLY;

	// スティックの値がデッドゾーンの値に収まっているか上下左右それぞれでチェック
	if (abs(x) > deadZoneL || abs(x) < -deadZoneL) {

		return true; // デッドゾーン外
	}
	if (abs(y) > deadZoneL || abs(y) < -deadZoneL) {

		return true; // デッドゾーン外
	}

	return false; // デッドゾーン内
}

Vector2 Input::GetRightThumbStick() {

	// コントローラーの右スティックの値を取得
	short x = controllerStateCurrent.Gamepad.sThumbRX;
	short y = controllerStateCurrent.Gamepad.sThumbRY;

	// スティックの値を-1.0fから1.0fの範囲に変換
	float xValue = static_cast<float>(x) / 32767.0f;
	float yValue = static_cast<float>(y) / 32767.0f;

	return Vector2(xValue, yValue);
}

Vector2 Input::GetLeftThumbStick() {

	// コントローラーの左スティックの値を取得
	short x = controllerStateCurrent.Gamepad.sThumbLX;
	short y = controllerStateCurrent.Gamepad.sThumbLY;

	// スティックの値を-1.0fから1.0fの範囲に変換
	float xValue = static_cast<float>(x) / 32767.0f;
	float yValue = static_cast<float>(y) / 32767.0f;

	return Vector2(xValue, yValue);
}

void Input::SetDeadZoneR(float deadZone) {
	this->deadZoneR = deadZone;
}

void Input::SetDeadZoneL(float deadZone) {
	this->deadZoneL = deadZone;
}
#include "Input.h"
// assertを使うため
#include <cassert>
// .libファイルのリンクをする
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

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
}

void Input::Update(){

	// 前回キーの入力を保存
	memcpy(keyPre, key, sizeof(key));

	// キーボード情報の取得開始
	keyboard->Acquire();
	// 全キーの入力情報を取得する
	keyboard->GetDeviceState(sizeof(key), key);
}

void Input::Finalize() {

	delete instance;
	instance = nullptr;
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
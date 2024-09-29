#include "Input.h"
// assertを使うため
#include <cassert>
// .libファイルのリンクをする
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

void Input::Initialize(HINSTANCE hInstance, HWND hwnd) {

	// 入力関数の初期化処理を移植する

	// WinMainのresult変数を用意
	HRESULT result;

	// hInstance : Windowsのウィンドウを作成するのに必要な「インスタンスハンドル」(実態はポインタ)
	// hwnd : Windowsのウィンドウをコントロールするのに必要な「ウィンドウハンドル」(実態はポインタ)
	
	// DirectInputのインスタンス生成
	ComPtr<IDirectInput8> directInput = nullptr;
	// 元データを参照して使う
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	// キーボードデバイス生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update(){

	// キーボード情報の取得開始
	keyboard->Acquire();
	// 全キーの入力情報を取得する
	keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber) {

	// 指定したキーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	// そうでなければ
	return false;
}
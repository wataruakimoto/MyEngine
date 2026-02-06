#include "Input.h"
#include "WinApp/WinApp.h"
#include "Math/MathVector.h"

#include <cassert>
#include <imgui.h>

// .libファイルのリンクをする
#pragma comment(lib,"xinput.lib")

using namespace Engine;
using namespace MathVector;

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

	// キーボードの初期化
	InitializeKeyboard();

	// コントローラーの初期化
	InitializeController();
}

void Input::Update() {

	// キーボードの更新
	UpdateKeyboard();

	// マウスの更新
	UpdateMouse();

	// コントローラーの更新
	UpdateController();
}

void Input::Finalize() {

	delete instance;
	instance = nullptr;
}

void Input::ShowImgui() {

#ifdef USE_IMGUI

	ImGui::Begin("Input"); // ウィンドウ開始

	// --- キーボード情報の表示 ---
	if (ImGui::TreeNode("Keyboard")) {
		ImGui::Text("Active Keys:");

		// キー名を判定するラムダ式
		auto GetKeyName = [](int key) -> std::string {
			// 数字 (0-9)
			if (key >= '0' && key <= '9') return std::string(1, (char)key);
			// アルファベット (A-Z)
			if (key >= 'A' && key <= 'Z') return std::string(1, (char)key);

			// よく使う特殊キー
			switch (key) {
			case VK_SPACE:		return "Space";
			case VK_RETURN:		return "Enter";
			case VK_BACK:		return "BackSpace";
			case VK_TAB:		return "Tab";
			case VK_ESCAPE:		return "Escape";
			case VK_SHIFT:		return "Shift";
			case VK_CONTROL:	return "Ctrl";
			case VK_MENU:		return "Alt";     // AltキーはVK_MENU
			case VK_UP:			return "Up";
			case VK_DOWN:		return "Down";
			case VK_LEFT:		return "Left";
			case VK_RIGHT:		return "Right";
			case VK_OEM_COMMA:	return ",";
			case VK_OEM_PERIOD:	return ".";
			case VK_OEM_MINUS:	return "-";
			case VK_OEM_PLUS:	return "+";
				// ファンクションキー
			case VK_F1: return "F1";  case VK_F2: return "F2";
			case VK_F3: return "F3";  case VK_F4: return "F4";
			case VK_F5: return "F5";  case VK_F6: return "F6";
			case VK_F7: return "F7";  case VK_F8: return "F8";
			case VK_F9: return "F9";  case VK_F10: return "F10";
			case VK_F11: return "F11"; case VK_F12: return "F12";
			}

			// それ以外はHex表記のままにする
			return "Unknown";
			};

		// 全キー(0~255)を走査して、何か反応があるキーだけ表示する
		for (int i = 0; i < 256; i++) {

			// キーの状態を取得
			bool isPush = PushKey((BYTE)i);
			bool isTrig = TriggerKey((BYTE)i);
			bool isRel = ReleaseKey((BYTE)i); 

			// 何かしらのアクションがあれば表示
			if (isPush || isTrig || isRel) {

				// キー名を取得
				std::string name = GetKeyName(i);

				// 名前が "Unknown" の場合は Hex も併記してわかりやすくする
				if (name == "Unknown") {
					ImGui::Text("Key [0x%02X] : %-7s %-7s %-7s",
						i,
						isPush ? "Push" : "",
						isTrig ? "Trigger" : "",
						isRel ? "Release" : ""
					);
				}
				else {
					// 名前がわかっている場合は名前を表示 (例: "Space")
					ImGui::Text("Key %-10s : %-7s %-7s %-7s",
						name.c_str(),
						isPush ? "Push" : "",
						isTrig ? "Trigger" : "",
						isRel ? "Release" : ""
					);
				}
			}
		}
		ImGui::TreePop();
	}

	// --- マウス情報の表示 ---
	if (ImGui::TreeNode("Mouse")) {
		// 座標
		ImGui::Text("Position: (%.1f, %.1f)", mousePosition.x, mousePosition.y);
		ImGui::Text("Move    : (%.1f, %.1f)", mouseMove.x, mouseMove.y);

		// ボタン状態
		// 0:Left, 1:Right, 2:Middle
		const char* buttonNames[] = { "Left", "Right", "Middle" };
		for (int i = 0; i < 3; i++) {
			bool isPush = mouseButton[i];
			bool isTrig = (!mouseButtonPre[i] && mouseButton[i]);

			ImGui::Text("%-6s : %s %s",
				buttonNames[i],
				isPush ? "Push" : "    ",
				isTrig ? "Trig" : "    "
			);
		}
		ImGui::TreePop();
	}

	// --- コントローラー情報の表示 ---
	if (ImGui::TreeNode("Controller")) {
		if (IsControllerConnected()) {
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Connected"); // 緑色で表示

			// スティックの値
			Vector2 left = GetLeftThumbStick();
			Vector2 right = GetRightThumbStick();
			ImGui::Text("L Stick: (%.2f, %.2f)", left.x, left.y);
			ImGui::Text("R Stick: (%.2f, %.2f)", right.x, right.y);

			// トリガーの値
			ImGui::Text("L Trigger: %d", controllerState.Gamepad.bLeftTrigger);
			ImGui::Text("R Trigger: %d", controllerState.Gamepad.bRightTrigger);

			// ボタン一覧
			// 表示したいボタンのリスト
			struct BtnInfo { WORD mask; const char* name; };
			BtnInfo buttons[] = {
				{ XINPUT_GAMEPAD_A, "A" },
				{ XINPUT_GAMEPAD_B, "B" },
				{ XINPUT_GAMEPAD_X, "X" },
				{ XINPUT_GAMEPAD_Y, "Y" },
				{ XINPUT_GAMEPAD_LEFT_SHOULDER, "LB" },
				{ XINPUT_GAMEPAD_RIGHT_SHOULDER, "RB" },
				{ XINPUT_GAMEPAD_DPAD_UP, "UP" },
				{ XINPUT_GAMEPAD_DPAD_DOWN, "DOWN" },
				{ XINPUT_GAMEPAD_DPAD_LEFT, "LEFT" },
				{ XINPUT_GAMEPAD_DPAD_RIGHT, "RIGHT" },
				{ XINPUT_GAMEPAD_START, "START" },
				{ XINPUT_GAMEPAD_BACK, "BACK" },
				{ XINPUT_GAMEPAD_LEFT_THUMB, "LTHUMB" },
				{ XINPUT_GAMEPAD_RIGHT_THUMB, "RTHUMB" },
			};

			ImGui::Text("Buttons:");
			// 横並びで表示するためのカウンタ
			int count = 0;
			for (const auto& btn : buttons) {
				bool push = PushButton(btn.mask);
				if (push) {
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "[%s]", btn.name);
					count++;
				}
			}
			if (count == 0) ImGui::Text("(None)");
		}
		else {
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Disconnected");
		}
		ImGui::TreePop();
	}

	ImGui::End();

#endif // USE_IMGUI
}

bool Input::PushKey(BYTE keyNumber) {

	// 指定したキーが押されていればtrue
	if (key[keyNumber] & 0x80) return true;

	// そうでなければfalse
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) {

	// 指定したキーがトリガーしていればtrue
	if ((key[keyNumber] & 0x80) && !(keyPre[keyNumber] & 0x80)) return true;

	// そうでなければfalse
	return false;
}

bool Input::ReleaseKey(BYTE keyNumber) {

	// 指定したキーがリリースしていればtrue
	if (!(key[keyNumber] & 0x80) && (keyPre[keyNumber] & 0x80)) return true;

	// そうでなければfalse
	return false;
}

const Vector2& Input::GetMousePosition() const {

	return mousePosition;
}

const Vector2& Input::GetMouseMove() const {

	return mouseMove;
}

bool Input::PushMouseButton(MouseButton button) {

	// 指定したマウスボタンが押されていればtrue
	if (mouseButton[static_cast<int>(button)]) return true;

	// そうでなければfalse
	return false;
}

bool Input::TriggerMouseButton(MouseButton button) {

	// 指定したマウスボタンがトリガーしていればtrue
	if (mouseButton[static_cast<int>(button)] && !mouseButtonPre[static_cast<int>(button)]) return true;

	// そうでなければfalse
	return false;
}

bool Input::ReleaseMouseButton(MouseButton button) {

	// 指定したマウスボタンがリリースしていればtrue
	if (!mouseButton[static_cast<int>(button)] && mouseButtonPre[static_cast<int>(button)]) return true;

	// そうでなければfalse
	return false;
}

bool Input::IsControllerConnected() const {
	
	return isConnected;
}

bool Input::PushButton(WORD button) {

	// コントローラーが接続されていなければfalseを返す
	if (!isConnected) return false;

	// 指定したボタンが押されていればtrue
	if (controllerState.Gamepad.wButtons & button) return true;

	// そうでなければfalse
	return false;
}

bool Input::TriggerButton(WORD button) {
	
	// コントローラーが接続されていなければfalseを返す
	if (!isConnected) return false;

	// 指定したボタンがトリガーしていればtrue
	if ((controllerState.Gamepad.wButtons & button) && !(controllerStatePre.Gamepad.wButtons & button)) return true;

	// そうでなければfalse
	return false;
}

bool Input::ReleaseButton(WORD button) {

	// コントローラーが接続されていなければfalseを返す
	if (!isConnected) return false;

	// 指定したボタンがリリースしていればtrue
	if (!(controllerState.Gamepad.wButtons & button) && (controllerStatePre.Gamepad.wButtons & button)) return true;

	// そうでなければfalse
	return false;
}

Vector2 Input::GetLeftThumbStick() {

	// コントローラーが接続されていなければ(0,0)を返す
	if (!isConnected) return Vector2(0.0f, 0.0f);

	// 左スティックの値を取得
	short x = controllerState.Gamepad.sThumbLX;
	short y = controllerState.Gamepad.sThumbLY;

	// スティックの値をfloatに変換して正規化(-1.0f ~ 1.0f)
	float xValue = static_cast<float>(x) / 32767.0f;
	float yValue = static_cast<float>(y) / 32767.0f;

	// デッドゾーンの処理
	if (std::abs(xValue) < deadZoneL && std::abs(yValue) < deadZoneL) {

		return Vector2(0.0f, 0.0f);
	}

	return Vector2(xValue, yValue);
}

Vector2 Input::GetRightThumbStick() {

	// コントローラーが接続されていなければ(0,0)を返す
	if (!isConnected) return Vector2(0.0f, 0.0f);

	// 右スティックの値を取得
	short x = controllerState.Gamepad.sThumbRX;
	short y = controllerState.Gamepad.sThumbRY;

	// スティックの値をfloatに変換して正規化(-1.0f ~ 1.0f)
	float xValue = static_cast<float>(x) / 32767.0f;
	float yValue = static_cast<float>(y) / 32767.0f;

	// デッドゾーンの処理
	if (std::abs(xValue) < deadZoneR && std::abs(yValue) < deadZoneR) {

		return Vector2(0.0f, 0.0f);
	}

	return Vector2(xValue, yValue);
}

void Input::SetDeadZone(float left, float right) {

	deadZoneL = left;
	deadZoneR = right;
}

void Input::InitializeKeyboard() {

	// 配列をクリアしておく
	memset(key, 0, sizeof(key));
	memset(keyPre, 0, sizeof(keyPre));
}

void Input::UpdateKeyboard() {

	// 前回キーの入力を保存
	memcpy(keyPre, key, sizeof(key));

	// WindowsAPIから全キーの状態を取得
	if (GetKeyboardState(key) == 0) {

		// 配列をクリア
		memset(key, 0, sizeof(key));
	}
}

void Input::UpdateMouse() {

	/// ===== 座標の更新 ===== ///

	// 前フレームのマウス座標を保存
	mousePositionPre = mousePosition;

	POINT point;

	// マウスカーソルの画面座標を取得
	if (GetCursorPos(&point)) {

		// 画面座標をクライアント座標に変換
		ScreenToClient(winApp_->GetHwnd(), &point);

		// マウスカーソルの位置を更新
		mousePosition = Vector2(static_cast<float>(point.x), static_cast<float>(point.y));
	}

	// マウスの移動量を計算
	mouseMove = mousePosition - mousePositionPre;

	/// ===== ボタンの更新 ===== ///

	for (int i = 0; i < 3; i++) {

		// マウスボタンの状態をコピー
		mouseButtonPre[i] = mouseButton[i];
	}

	// 各ボタンの状態を取得
	mouseButton[static_cast<int>(MouseButton::Left)] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	mouseButton[static_cast<int>(MouseButton::Right)] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
	mouseButton[static_cast<int>(MouseButton::Middle)] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
}

void Input::InitializeController() {

	// コントローラーの状態の初期化
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
	ZeroMemory(&controllerStatePre, sizeof(XINPUT_STATE));
}

void Input::UpdateController() {

	// 前フレームの状態を保存
	controllerStatePre = controllerState;

	// コントローラーの状態を取得
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
	DWORD result = XInputGetState(0, &controllerState); // コントローラー0を取得

	// コントローラーが接続されていたら
	if (result == ERROR_SUCCESS) {

		isConnected = true;
	}
	// コントローラーが接続されていなかったら
	else {

		isConnected = false;

		// 状態をクリア
		ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
	}
}

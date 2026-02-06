#pragma once

#include "Vector2.h"

#include <Windows.h> // WindowsAPIを使うため
#include <wrl.h> // Comptrを使うため
#include <Xinput.h> // XInputを使うため

namespace Engine {

	/// ===== 列挙型 ===== ///

	// マウスボタン
	enum class MouseButton {
		Left = 0, // 左クリック
		Right,	  // 右クリック
		Middle,	  // 中クリック(ホイールクリック)
	};

	/// ===== 前方宣言 ===== ///
	class WinApp;

	/// ===== 入力 ===== ///
	class Input {

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

		///-------------------------------------------/// 
		/// キーボード操作関数
		///-------------------------------------------///

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
		/// キーのリリースをチェック
		/// </summary>
		/// <param name="keyNumber">キー番号</param>
		/// <returns>リリースしているか</returns>
		bool ReleaseKey(BYTE keyNumber);

		///-------------------------------------------///
		/// マウス操作関数
		///-------------------------------------------///

			/// <summary>
			/// マウスカーソルの位置を取得
			/// </summary>
			/// <returns></returns>
		const Vector2& GetMousePosition() const;

		/// <summary>
		/// マウスの移動量を取得
		/// </summary>
		/// <returns></returns>
		const Vector2& GetMouseMove() const;

		/// <summary>
		/// マウスボタンの押下をチェック
		/// </summary>
		/// <param name="button">マウスボタン</param>
		/// <returns></returns>
		bool PushMouseButton(MouseButton button);

		/// <summary>
		/// マウスボタンのトリガーをチェック
		/// </summary>
		/// <param name="button">マウスボタン</param>
		/// <returns></returns>
		bool TriggerMouseButton(MouseButton button);

		/// <summary>
		/// マウスボタンのリリースをチェック
		/// </summary>
		/// <param name="button">マウスボタン</param>
		/// <returns></returns>
		bool ReleaseMouseButton(MouseButton button);

		///-------------------------------------------///
		/// コントローラー操作関数
		///-------------------------------------------///

			/// <summary>
			/// コントローラーが接続されているか
			/// </summary>
			/// <returns></returns>
		bool IsControllerConnected() const;

		/// <summary>
		/// コントローラーの押下をチェック
		/// </summary>
		/// <param name="button">ボタン</param>
		/// <returns></returns>
		bool PushButton(WORD button);

		/// <summary>
		/// コントローラーのトリガーをチェック
		/// </summary>
		/// <param name="button">ボタン</param>
		/// <returns></returns>
		bool TriggerButton(WORD button);

		/// <summary>
		/// コントローラーのリリースをチェック
		/// </summary>
		/// <param name="button">ボタン</param>
		/// <returns></returns>
		bool ReleaseButton(WORD button);

		/// <summary>
		/// 左スティックの値を取得
		/// </summary>
		Vector2 GetLeftThumbStick();

		/// <summary>
		/// 右スティックの値を取得
		/// </summary>
		Vector2 GetRightThumbStick();

		/// <summary>
		/// デッドゾーンの設定(0.0f ~ 1.0f)
		/// </summary>
		/// <param name="left">左のデッドゾーン</param>
		/// <param name="right">右のデッドゾーン</param>
		void SetDeadZone(float left, float right);

		///-------------------------------------------/// 
		/// クラス内関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// キーボードの初期化
		/// </summary>
		void InitializeKeyboard();

		/// <summary>
		/// キーボードの更新
		/// </summary>
		void UpdateKeyboard();

		/// <summary>
		/// マウスの更新
		/// </summary>
		void UpdateMouse();

		/// <summary>
		/// コントローラーの初期化
		/// </summary>
		void InitializeController();

		/// <summary>
		/// コントローラーの更新
		/// </summary>
		void UpdateController();

		///-------------------------------------------/// 
		/// ゲッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static Input* GetInstance();

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		/// ===== キーボード用変数 ===== ///

		// 全キーの状態
		BYTE key[256] = {};
		// 前のフレームの全キーの状態
		BYTE keyPre[256] = {};

		/// ===== マウス用変数 ===== ///

		// マウスカーソルの座標
		Vector2 mousePosition = {};
		// 前のフレームのマウスカーソルの座標
		Vector2 mousePositionPre = {};
		// 移動量
		Vector2 mouseMove = {};

		// マウスボタンの状態
		bool mouseButton[3] = {};
		// 前のフレームのマウスボタンの状態
		bool mouseButtonPre[3] = {};

		/// ===== コントローラー用変数 ===== ///

		// コントローラーが接続フラグ
		bool isConnected = false;

		// 現在のコントローラーの状態
		XINPUT_STATE controllerState;
		// 前のフレームのコントローラーの状態
		XINPUT_STATE controllerStatePre;

		// デッドゾーンの値 (0.0f ~ 1.0f)
		float deadZoneL = 0.24f; // 左のデッドゾーン
		float deadZoneR = 0.26f; // 右のデッドゾーン

		/// ===== 借りポインタ・インスタンス ===== ///

		// WindowsAPIの借りポインタ
		WinApp* winApp_ = nullptr;
	};
}
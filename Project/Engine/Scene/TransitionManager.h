#pragma once

#include <memory>
#include <functional>

namespace Engine {

	/// ===== 前方宣言 ===== ///

	class BaseTransition;

	/// <summary>
	/// シーン遷移の管理クラス
	/// </summary>
	class TransitionManager {

	///-------------------------------------------/// 
	/// シングルトン
	///-------------------------------------------///
	private:

		// インスタンス
		static TransitionManager* instance;

		// コンストラクタの隠蔽
		TransitionManager() = default;
		// デストラクタの隠蔽
		~TransitionManager() = default;
		// コピーコンストラクタの封印
		TransitionManager(TransitionManager&) = delete;
		// コピー代入演算子の封印
		TransitionManager& operator=(TransitionManager&) = delete;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
	public:

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		/// <summary>
		/// 遷移の開始
		/// </summary>
		/// <param name="transition">遷移の仕方</param>
		/// <param name="onComplete">遷移完了時の処理</param>
		/// <param name="duration">遷移にかける時間</param>
		void StartTransition(
			std::unique_ptr<BaseTransition> transition, 
			std::function<void()> onComplete = nullptr, 
			float duration = 1.0f
		);

	///-------------------------------------------/// 
	/// ゲッター
	///-------------------------------------------///
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		static TransitionManager* GetInstance();
		
	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
	private:

		/// <summary>
		/// 遷移の状態
		/// </summary>
		enum class TransitionState {
			None, // 遷移なし
			Out,  // アウト (遷移を開始している状態)
			In,	  // イン (遷移が完了している状態)
		};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// 遷移の状態
		TransitionState state_ = TransitionState::None;

		// 現在の遷移
		std::unique_ptr<BaseTransition> currentTransition_ = nullptr;

		// 遷移完了後に呼び出すコールバック関数
		std::function<void()> onTransitionComplete_ = nullptr;

		// 遷移の進行度 (0 ~ 1)
		float progress_ = 0.0f;

		// 遷移の総時間 (秒)
		float duration_ = 1.0f;

		const float deltaTime_ = 1.0f / 60.0f;
	};
}


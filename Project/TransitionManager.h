#pragma once

namespace Engine {

	/// <summary>
	/// 画面遷移マネージャー
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

		void Initialize();

		void Update();

		void Finalize();

		void ShowImGui();


	///-------------------------------------------/// 
	///
	///-------------------------------------------///
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static TransitionManager* GetInstance();
	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:
	};
}


#pragma once

namespace Engine {

	/// <summary>
	/// シーン遷移の基底クラス
	/// </summary>
	class BaseTransition {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
	public:
		
		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~BaseTransition() = default;

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="progress">遷移の進行度 (0 ~ 1)</param>
		virtual void Update(float progress) = 0;

		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// ImGui表示
		/// </summary>
		virtual void ShowImGui() = 0;
	};
}

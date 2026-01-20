#pragma once

/// ===== シーン基底 ===== ///
class BaseScene {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// フィルター適応のある描画
	/// </summary>
	virtual void DrawFiltered() = 0;

	/// <summary>
	/// フィルター適応のない描画
	/// </summary>
	virtual void DrawUnfiltered() = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// ImGui表示
	/// </summary>
	virtual void ShowImGui() = 0;
};
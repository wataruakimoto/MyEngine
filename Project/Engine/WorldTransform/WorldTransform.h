#pragma once
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"

#include <d3d12.h>
#include <wrl.h>

class Camera;

/// ===== ワールド変換 ===== ///
class WorldTransform {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui();

	/// <summary>
	/// 平行移動に加算
	/// </summary>
	/// <param name="value"></param>
	void AddTranslate(const Vector3& value);

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// 拡縮のゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3& GetScale() const { return scale_; }

	/// <summary>
	/// 回転のゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3& GetRotate() const { return rotate_; }

	/// <summary>
	/// 平行移動のゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3& GetTranslate() const { return translate_; }

	/// <summary>
	/// ワールド行列のゲッター
	/// </summary>
	/// <returns></returns>
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const;

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// 拡縮のセッター
	/// </summary>
	/// <param name="scale">拡縮</param>
	void SetScale(const Vector3& scale) { scale_ = scale; }

	/// <summary>
	/// 回転のセッター
	/// </summary>
	/// <param name="rotate">回転</param>
	void SetRotate(const Vector3& rotate) { rotate_ = rotate; }

	/// <summary>
	/// 平行移動のセッター
	/// </summary>
	/// <param name="translate">平行移動</param>
	void SetTranslate(const Vector3& translate) { translate_ = translate; }

	/// <summary>
	/// 親のワールド変換クラスのセッター
	/// </summary>
	/// <param name="parent">親のワールド変換クラス</param>
	void SetParent(const WorldTransform* parent) { parent_ = parent; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 拡縮
	Vector3 scale_;

	// 回転
	Vector3 rotate_;

	// 平行移動
	Vector3 translate_;

	// ワールド行列
	Matrix4x4 worldMatrix_;

	// 親のワールド変換クラス
	const WorldTransform* parent_;
};
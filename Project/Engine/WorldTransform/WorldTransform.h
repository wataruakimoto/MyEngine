#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

#include <d3d12.h>
#include <wrl.h>

/// ===== 前方宣言 ===== ///
class DirectXUtility;
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
	/// 行列の更新
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// 行列の転送
	/// </summary>
	void TransferMatrix();

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

	/// <summary>
	/// DirectXユーティリティのセッター
	/// </summary>
	void SetDirectXUtility(DirectXUtility* dxUtility) { dxUtility_ = dxUtility; }

	/// <summary>
	/// カメラのセッター
	/// </summary>
	void SetCamera(Camera* camera) { camera_ = camera; }

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	// 座標変換
	struct Transformation {
		Matrix4x4 worldMatrix;  // ワールド行列
		Matrix4x4 WVPMatrix;	// ワールドビュープロジェクション行列
		Matrix4x4 worldInverseTranspose; // ワールド逆転置行列
	};

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 拡縮
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f };

	// 回転
	Vector3 rotate_ = { 0.0f, 0.0f, 0.0f };

	// 平行移動
	Vector3 translate_ = { 0.0f, 0.0f, 0.0f };

	// ワールド行列
	Matrix4x4 worldMatrix_ = {};

	// ワールドビュープロジェクション行列
	Matrix4x4 WVPMatrix_ = {};

	// 親のワールド変換クラス
	const WorldTransform* parent_ = nullptr;

	// 座標変換データ
	Transformation transformationData_ = {};

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_ = nullptr;

	// DirectXUtilityのインスタンス
	DirectXUtility* dxUtility_ = nullptr;

	// カメラの借りポインタ
	Camera* camera_ = nullptr;
};
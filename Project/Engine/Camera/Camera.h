#pragma once

#include "math/Vector3.h"
#include "math/Matrix4x4.h"

/// === カメラ === ///
class Camera {

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	// 変換データ
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 位置データ
	Transform transform;

	// ワールド行列
	Matrix4x4 worldMatrix;

	// ビュー行列
	Matrix4x4 viewMatrix;

	// プロジェクション行列
	Matrix4x4 projectionMatrix;

	// プロジェクション行列の逆行列
	Matrix4x4 projectionMatrixInverse;

	// ビュープロジェクション行列
	Matrix4x4 viewProjectionMatrix;

	// ワールド座標
	Vector3 worldPosition = {};

	// 水平方向視野 (field of view)
	float fovY;

	// アスペクト比
	float aspectRatio;

	// ニアクリップ距離
	float nearClip;

	// ファークリップ距離
	float farClip;

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui表示
	/// </summary>
	/// <param name="name"></param>
	void ShowImGui(const char* name);

///-------------------------------------------///
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// 回転のセッター
	/// </summary>
	/// <param name="rotate">回転</param>
	void SetRotate(const Vector3& rotate) { this->transform.rotate = rotate; }

	/// <summary>
	/// 位置のセッター
	/// </summary>
	/// <param name="translate">位置</param>
	void SetTranslate(const Vector3& translate) { this->transform.translate = translate; }

	/// <summary>
	/// 水平方向視野のセッター
	/// </summary>
	/// <param name="fovY">水平方向視野</param>
	void SetFovY(float fovY) { this->fovY = fovY; }

	/// <summary>
	/// アスペクト比のセッター
	/// </summary>
	/// <param name="aspectRatio">アスペクト比</param>
	void SetAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; }

	/// <summary>
	/// ニアクリップ距離のセッター
	/// </summary>
	/// <param name="nearClip">ニアクリップ距離</param>
	void SetNearClip(float nearClip) { this->nearClip = nearClip; }

	/// <summary>
	/// ファークリップ距離のセッター
	/// </summary>
	/// <param name="farClip">ファークリップ距離</param>
	void SetFarClip(float farClip) { this->farClip = farClip; }

///-------------------------------------------///
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// 回転のゲッター
	/// </summary>
	/// <returns>Vector3</returns>
	const Vector3& GetRotate() const { return transform.rotate; }

	/// <summary>
	/// 位置のゲッター
	/// </summary>
	/// <returns>Vector3</returns>
	const Vector3& GetTranslate() const { return transform.translate; }

	/// <summary>
	/// ワールド行列のゲッター
	/// </summary>
	/// <returns>Matrix4x4</returns>
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix; }

	/// <summary>
	/// ビュー行列のゲッター
	/// </summary>
	/// <returns>Matrix4x4</returns>
	const Matrix4x4& GetViewMatrix() const { return viewMatrix; }

	/// <summary>
	/// プロジェクション行列のゲッター
	/// </summary>
	/// <returns>Matrix4x4</returns>
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix; }

	/// <summary>
	/// プロジェクション行列の逆行列のゲッター
	/// </summary>
	/// <returns></returns>
	const Matrix4x4& GetProjectionMatrixInverse() const { return projectionMatrixInverse; }

	/// <summary>
	/// ビュープロジェクション行列のゲッター
	/// </summary>
	/// <returns>Matrix4x4</returns>
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }

	/// <summary>
	/// ワールド座標のゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3& GetWorldPosition() const { return worldPosition; }
};
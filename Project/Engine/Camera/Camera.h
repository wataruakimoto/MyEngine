#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"
#include "WorldTransform.h"

namespace Engine {

	/// === カメラ === ///
	class Camera {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// ビュー行列の更新
		/// </summary>
		void UpdateViewMatrix();

		/// <summary>
		/// プロジェクション行列の更新
		/// </summary>
		void UpdateProjectionMatrix();

		/// <summary>
		/// ビュープロジェクション行列の更新
		/// </summary>
		void UpdateViewProjectionMatrix();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// ImGui表示
		/// </summary>
		/// <param name="name"></param>
		void ShowImGui(const char* name);

		/// <summary>
		/// ImGuiツリー表示
		/// </summary>
		void ShowImGuiTree();

	/// ================================================== ///
	/// セッター
	/// ================================================== ///
	public:

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

		/// <summary>
		/// ビュー行列のセッター
		/// </summary>
		/// <param name="viewMatrix"></param>
		void SetViewMatrix(const Matrix4x4& viewMatrix) { this->viewMatrix = viewMatrix; }

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
	public:

		/// <summary>
		/// ワールド変換のゲッター
		/// </summary>
		/// <returns>WorldTransform</returns>
		WorldTransform& GetWorldTransform() { return worldTransform_; }

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
		/// ニアクリップ距離のゲッター
		/// </summary>
		/// <returns>ニアクリップ距離	</returns>
		const float& GetNearClip() const { return nearClip; }

		/// <summary>
		/// ファークリップ距離のゲッター
		/// </summary>
		/// <returns>ファークリップ距離</returns>
		const float& GetFarClip() const { return farClip; }

		/// <summary>
		/// 水平方向視野のゲッター
		/// </summary>
		/// <returns>水平方向視野</returns>
		const float& GetFovY() const { return fovY; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
	private:

		// ワールド変換
		WorldTransform worldTransform_;

		// ビュー行列
		Matrix4x4 viewMatrix;

		// プロジェクション行列
		Matrix4x4 projectionMatrix;

		// プロジェクション行列の逆行列
		Matrix4x4 projectionMatrixInverse;

		// ビュープロジェクション行列
		Matrix4x4 viewProjectionMatrix;

		// 水平方向視野 (field of view)
		float fovY;

		// アスペクト比
		float aspectRatio;

		// ニアクリップ距離
		float nearClip;

		// ファークリップ距離
		float farClip;
	};
}
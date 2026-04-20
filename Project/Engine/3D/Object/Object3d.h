#pragma once

#include "Model/Model.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "WorldTransform.h"

#include <fstream>
#include <vector>
#include <wrl.h>

namespace Engine {

	/// === 前方宣言 === ///
	class Camera;
	class DirectXUtility;
	class Object3dRenderer;

	/// === 3Dオブジェクト === ///
	class Object3d {

	/// ================================================== ///
	/// 構造体
	/// ================================================== ///
	private:

		// 座標変換行列
		struct TransformationMatrix {
			Matrix4x4 WVP;
			Matrix4x4 world;
			Matrix4x4 worldInverseTranspose;
		};

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
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
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// ImGui表示
		/// </summary>
		void ShowImGui();

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
	private:

		/// <summary>
		/// 座標変換行列データ初期化
		/// </summary>
		void InitializeTransformationMatrixData();

		/// <summary>
		/// カメラデータ初期化
		/// </summary>
		void InitializeCameraData();

	/// ================================================== ///
	/// セッター
	/// ================================================== ///
	public:

		/// <summary>
		/// モデルのセッター
		/// </summary>
		/// <param name="model">モデル</param>
		void SetModel(Model* model) { this->model = model; }

		/// <summary>
		/// カメラのセッター
		/// </summary>
		/// <param name="camera">カメラ</param>
		void SetCamera(Camera* camera) { this->camera = camera; }

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
	public:

		/// <summary>
		/// ワールド座標変換の取得
		/// </summary>
		/// <returns>ワールド座標変換</returns>
		WorldTransform& GetWorldTransform() { return worldTransform; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
	private:

		// 座標変換行列リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;

		// カメラリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource;

		// 座標変換行列データ
		TransformationMatrix* transformationMatrixData = nullptr;
		
		// カメラデータ
		Vector3* cameraData;

		// モデル
		Model* model = nullptr;

		// ワールド変換
		WorldTransform worldTransform;

		// カメラ
		Camera* camera = nullptr;

		bool isDraw = true;

		// DirectXUtilityのインスタンス
		DirectXUtility* dxUtility = nullptr;

		// Object3dRendererのインスタンス
		Object3dRenderer* object3dRenderer_ = nullptr;
	};
}
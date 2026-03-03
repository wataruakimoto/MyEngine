#pragma once

#include "Base/DirectXUtility.h"
#include "Data/TransformationData.h"
#include "Vector2.h"
#include "Vector4.h"
#include "WorldTransform.h"

#include <d3d12.h>
#include <wrl.h>
#include <stdint.h>
#include <string>

namespace Engine {

	/// === 前方宣言 === ///
	class Camera;

	/// === スカイボックス === ///
	class Skybox {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="relativePath">相対パス</param>
		void Initialize(const std::string relativePath);

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
		/// <param name="name"></param>
		void ShowImGui(const char* name);

		///-------------------------------------------/// 
		/// クラス内関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// 頂点データの初期化
		/// </summary>
		void InitializeVertexData();

		/// <summary>
		/// インデックスデータの初期化
		/// </summary>
		void InitializeIndexData();

		/// <summary>
		/// 座標変換データの初期化
		/// </summary>
		void InitializeTransformationData();

		/// <summary>
		/// マテリアルデータの初期化
		/// </summary>
		void InitializeMaterialData();

		///-------------------------------------------/// 
		/// セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// カメラのセッター
		/// </summary>
		/// <param name="camera"></param>
		void SetCamera(Camera* camera) { this->camera = camera; }

		/// <summary>
		/// 拡縮のセッター
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale) { this->worldTransform.SetScale(scale); }

		/// <summary>
		/// 回転のセッター
		/// </summary>
		/// <param name="rotate"></param>
		void SetRotate(const Vector3& rotate) { this->worldTransform.SetRotate(rotate); }

		/// <summary>
		/// 平行移動のセッター
		/// </summary>
		/// <param name="translate"></param>
		void SetTranslate(const Vector3& translate) { this->worldTransform.SetTranslate(translate); }

		///-------------------------------------------/// 
		/// 構造体
		///-------------------------------------------///
	public:

		struct VertexData {
			Vector4 position; // 頂点座標
		};

		struct Material {
			Vector4 color; // 色
		};

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		// 頂点リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
		// 頂点データ
		VertexData* vertexData = nullptr;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};

		// インデックスリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
		// インデックスデータ
		uint32_t* indexData = nullptr;
		// インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW indexBufferView = {};

		// 座標変換リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource;
		// 座標変換データ
		TransformationData* transformationData = nullptr;

		// マテリアルリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
		// マテリアルデータ
		Material* materialData = nullptr;

		// 頂点数 8頂点
		uint32_t vertexCount = 8;

		// インデックス数 6頂点 * 6面 = 36頂点
		uint32_t indexCount = 36;

		// テクスチャのSRVインデックス
		uint32_t textureSrvIndex = 0;

		// 変換データ
		WorldTransform worldTransform = {};

		// カメラの借りポインタ
		Camera* camera = nullptr;

		// DirectXUtilityのインスタンス
		DirectXUtility* dxUtility_ = DirectXUtility::GetInstance();
	};
}

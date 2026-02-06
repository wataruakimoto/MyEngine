#pragma once

#include "Matrix4x4.h"
#include "Data/MaterialData.h"
#include "Data/ModelData.h"
#include "Data/VertexData.h"

#include <d3d12.h>
#include <wrl.h>
#include <memory>

namespace Engine {

	// === 前方宣言=== ///
	class ModelCommon;
	class DirectXUtility;

	/// === 3Dモデル === ///
	class Model {

		///-------------------------------------------/// 
		/// 構造体
		///-------------------------------------------///
	public:

		// マテリアルデータ
		struct Material {
			Vector4 color;
			int lightingMode;
			float padding[3];
			Matrix4x4 uvTransform;
			float shininess;
		};

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="directoryName">ディレクトリ名</param>
		/// <param name="fileName">ファイル名</param>
		void Initialize(const std::string& directoryName, const std::string& fileName);

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// ImGui表示
		/// </summary>
		void ShowImGui();

		///-------------------------------------------/// 
		/// クラス内関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// 頂点データ初期化
		/// </summary>
		void InitializeVertexData();

		/// <summary>
		/// マテリアルデータ初期化
		/// </summary>
		void InitializeMaterialData();

		///-------------------------------------------/// 
		/// ゲッター&セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// ライティングの種類のゲッター
		/// </summary>
		/// <returns></returns>
		const int& GetLightingMode() const { return materialData->lightingMode; }

		/// <summary>
		/// ライティングの種類のセッター
		/// </summary>
		/// <param name="lightingMode"></param>
		void SetLightingMode(int lightingMode) { materialData->lightingMode = lightingMode; }

		/// <summary>
		/// ライトの明るさのゲッター
		/// </summary>
		/// <returns></returns>
		const float& GetShininess() const { return materialData->shininess; }

		/// <summary>
		/// ライトの明るさのセッター
		/// </summary>
		/// <param name="shininess"></param>
		void SetShininess(const float& shininess) { this->materialData->shininess = shininess; }

		const Matrix4x4& GetRootMatrix() const { return modelData->rootNode.localMatrix; }

		/// <summary>
		/// 環境マップのファイルパスのゲッター
		/// </summary>
		/// <param name="filePath"></param>
		void SetEnvironmentMapFilePath(const std::string& filePath) { environmentMapFilePath = filePath; }

		const Vector4& GetPosition() const { return vertexData->position; }

		/// <summary>
		/// 色のセッター
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector4& color) { materialData->color = color; }

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		// objファイルのデータ
		ModelData* modelData = nullptr;

		// 頂点リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;

		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		// 頂点データ
		VertexData* vertexData = nullptr;

		// マテリアルリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;

		// マテリアルデータ
		Material* materialData = nullptr;

		// 環境マップ用のファイルパス
		std::string environmentMapFilePath = "Resources/Textures/rostock_laage_airport_4k.dds";

		// DirectXUtilityのインスタンス
		DirectXUtility* dxUtility = nullptr;
	};
}
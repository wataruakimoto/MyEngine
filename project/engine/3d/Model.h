#pragma once
#include "math/Matrix4x4.h"
#include "base/DirectXCommon.h"
#include "Data/MaterialData.h"
#include "Data/ModelData.h"
#include "Data/VertexData.h"

// === 3Dモデル共通部=== ///
class ModelCommon;

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
	void Initialize(const std::string& directorypath, const std::string& filename);

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

	const Matrix4x4& GetRootMatrix() const { return modelData.rootNode.localMatrix; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// objファイルのデータ
	ModelData modelData;

	// 頂点リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	// 頂点データ
	VertexData* vertexData = nullptr;

	// マテリアルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource;

	// マテリアルデータ
	Material* materialData = nullptr;
};
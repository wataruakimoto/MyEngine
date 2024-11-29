#pragma once

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"
#include "base/DirectXCommon.h"

// === 3Dモデル共通部=== ///
class ModelCommon;

/// === 3Dモデル === ///
class Model {

///=====================================================/// 
/// 構造体
///=====================================================///
public:

	// 頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	// マテリアルデータ
	struct Material {
		Vector4 color;
		bool enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	// マテリアルデータ
	struct MaterialData {
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};

	// モデルデータ
	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};

///=====================================================/// 
/// メンバ関数
///=====================================================///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelCommon">3Dモデル基盤</param>
	void Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// .mtlファイル読み込み
	/// </summary>
	/// <param name="directoryPath">ディレクトリーパス</param>
	/// <param name="filename">ファイル名</param>
	/// <returns></returns>
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// .objファイル読み込み
	/// </summary>
	/// <param name="directoryPath">ディレクトリーパス</param>
	/// <param name="filename">ファイル名</param>
	/// <returns></returns>
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

///=====================================================/// 
/// クラス内関数
///=====================================================///
private:

	/// <summary>
	/// 頂点データ初期化
	/// </summary>
	void InitializeVertexData();

	/// <summary>
	/// マテリアルデータ初期化
	/// </summary>
	void InitializeMaterialData();

///=====================================================/// 
/// メンバ変数
///=====================================================///
private:

	// ModelCommonのポインタ
	ModelCommon* modelCommon_ = nullptr;

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
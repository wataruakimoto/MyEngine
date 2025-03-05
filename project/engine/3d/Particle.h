#pragma once
#include "base/DirectXCommon.h"
#include "Data/MaterialData.h"
#include "Data/ModelData.h"
#include "Data/VertexData.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"

/// ===== カメラ ===== ///
class Camera;

/// ===== パーティクル ===== ///
class Particle {

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	// 座標変換行列
	struct ShaderTransform {
		Matrix4x4 WVP;
		Matrix4x4 world;
		Vector4 color;
	};

	// 変換データ
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

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

	/// <summary>
	/// ランダム生成
	/// </summary>
	void CreateRandom();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
public:

	/// <summary>
	/// 座標変換行列データ初期化
	/// </summary>
	void InitializeShaderTransformData();

	/// <summary>
	/// カメラデータ初期化
	/// </summary>
	void InitializeCameraData();

	/// <summary>
	/// 頂点データ初期化
	/// </summary>
	void InitializeVertexData();

	/// <summary>
	/// マテリアルデータ初期化
	/// </summary>
	void InitializeMaterialData();

	/// <summary>
	/// SRV作成
	/// </summary>
	void CreateSRVForTransformationMatrix();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera) { this->camera = camera; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 座標変換行列リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> ShaderTransformResource;
	// カメラリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> cameraResource;

	// 頂点リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource;
	// マテリアルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource;

	// 座標変換行列データ
	ShaderTransform* ShaderTransformData = nullptr;
	// カメラデータ
	Vector3* cameraData;

	// 頂点データ
	VertexData* vertexData = nullptr;
	// マテリアルデータ
	Material* materialData = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	// カメラ
	Camera* camera = nullptr;

	// objファイルのデータ
	ModelData modelData;

	bool isDraw = true;

	// インスタンス数
	static const int kNumMaxInstance = 10;
	// 描画すべきインスタンスの数
	uint32_t numInstance = 0;

	// SRVインデックス
	uint32_t srvIndex;

	Transform transform[kNumMaxInstance];

	Vector3 velocity[kNumMaxInstance];

	Vector4 color[kNumMaxInstance];

	float lifeTime[kNumMaxInstance];
	float currentTime[kNumMaxInstance];

	// Δt
	const float kDeltaTime = 1.0f / 60.0f;

	Matrix4x4 backToFrontMatrix = {};
};


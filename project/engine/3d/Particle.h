#pragma once
#include "Data/MaterialData.h"
#include "Data/ModelData.h"
#include "Data/VertexData.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"
#include "base/DirectXCommon.h"

#include <fstream>
#include <vector>

/// === 前方宣言 === ///
class Camera;

/// === パーティクル === ///
class Particle {

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

	// 座標変換行列
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 world;
		Matrix4x4 worldInverseTranspose;
	};

	// 平行光源データ
	struct DirectionalLight {
		Vector4 color; // 色
		Vector3 direction; // 向き
		float intensity; // 輝度
	};

	// 点光源データ
	struct PointLight {
		Vector4 color; // 色
		Vector3 position; // 位置
		float intensity; // 輝度
		float distance; // 光の届く最大距離
		float decay; // 減衰率
		float padding[2];
	};

	// スポットライトデータ
	struct SpotLight {
		Vector4 color; // 色
		Vector3 position; // 位置
		float padding;
		Vector3 direction; // 向き
		float intensity; // 輝度
		float distance; // 光の届く最大距離
		float decay; // 減衰率
		float cosAngle; // 余弦
		float cosFalloffStart; // Falloffの開始角度
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
	void ShowImGui(const char* name);

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera) { this->camera = camera; }

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// 座標変換行列データ初期化
	/// </summary>
	void InitializeTransformationMatrixData();

	/// <summary>
	/// 平行光源データ初期化
	/// </summary>
	void InitializeDirectionalLightData();

	/// <summary>
	/// 点光源データ初期化
	/// </summary>
	void InitializePointLightData();

	/// <summary>
	/// スポットライトデータ初期化
	/// </summary>
	void InitializeSpotLightData();

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

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 座標変換行列リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> transformationMatrixResource;
	// 平行光源リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> directionalLightResource;
	// 点光源リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> pointLightResource;
	// スポットライトリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> spotLightResource;
	// カメラリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> cameraResource;

	// 頂点リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource;
	// マテリアルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource;

	// 座標変換行列データ
	TransformationMatrix* transformationMatrixData = nullptr;
	// 平行光源データ
	DirectionalLight* directionalLightData = nullptr;
	// 点光源データ
	PointLight* pointLightData = nullptr;
	// スポットライトデータ
	SpotLight* spotLightData = nullptr;
	// カメラデータ
	Vector3* cameraData;

	// 頂点データ
	VertexData* vertexData = nullptr;
	// マテリアルデータ
	Material* materialData = nullptr;

	// objファイルのデータ
	ModelData modelData;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	
	// カメラ
	Camera* camera = nullptr;

	bool isDraw = true;

	Transform transform;
	Vector3 velocity;
	float lifeTime;
	float currentTime;

	const int kNumInstance = 10; // インスタンス数
};
#pragma once
#include "base/DirectXCommon.h"
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

	// 変換データ
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
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
	/// <param name="name"></param>
	void ShowImGui(const char* name);

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
public:

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

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

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

	Transform transform;

	// カメラ
	Camera* camera = nullptr;

	bool isDraw = true;
};


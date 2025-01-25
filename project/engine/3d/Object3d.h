#pragma once

#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"
#include <fstream>
#include <vector>
#include "base/DirectXCommon.h"
#include "Model.h"

/// === カメラ === ///
class Camera;

/// === 3Dオブジェクト === ///
class Object3d {

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

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// 大きさのセッター
	/// </summary>
	/// <param name="scale">大きさ</param>
	void SetScale(const Vector3& scale) { this->transform.scale = scale; }

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
	/// モデルのセッター
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { this->model = model; }

	/// <summary>
	/// モデルのセッター
	/// </summary>
	/// <param name="filePath"></param>
	void SetModel(const std::string& filePath);

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera) { this->camera = camera; }

	/// <summary>
	/// 色のセッター
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color) { this->directionalLightData->color = color; }

	/// <summary>
	///	向きのセッター
	/// </summary>
	/// <param name="direction"></param>
	void SetDirection(const Vector3& direction) { this->directionalLightData->direction = direction; }

	/// <summary>
	/// 輝度のセッター
	/// </summary>
	/// <param name="intensity"></param>
	void SetIntensity(float intensity) { this->directionalLightData->intensity = intensity; }

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// 大きさのゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3& GetScale() const { return transform.scale; }

	/// <summary>
	/// 回転のゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3& GetRotate() const { return transform.rotate; }

	/// <summary>
	/// 位置のゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3& GetTranslate() const { return transform.translate; }

	/// <summary>
	/// 色のゲッター
	/// </summary>
	/// <returns></returns>
	const Vector4& GetColor() const { return directionalLightData->color; }

	/// <summary>
	/// 向きのゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3& GetDirection() const { return directionalLightData->direction; }

	/// <summary>
	/// 輝度のゲッター
	/// </summary>
	/// <returns></returns>
	float GetIntensity() const { return directionalLightData->intensity; }

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

	// モデル
	Model* model = nullptr;

	// カメラ
	Camera* camera = nullptr;
};
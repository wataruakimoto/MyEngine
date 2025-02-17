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

	///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// 座標変換行列データ初期化
	/// </summary>
	void InitializeTransformationMatrixData();

	/// <summary>
	/// 頂点データ初期化
	/// </summary>
	void InitializeVertexData();

	/// <summary>
	/// マテリアルデータ初期化
	/// </summary>
	void InitializeMaterialData();

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
	/// カメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera) { this->camera = camera; }

	/// <summary>
	/// ライティングの種類のセッター
	/// </summary>
	/// <param name="lightingMode"></param>
	void SetLightingMode(int lightingMode) { materialData->lightingMode = lightingMode; }

	/// <summary>
	/// ライトの明るさのセッター
	/// </summary>
	/// <param name="shininess"></param>
	void SetShininess(const float& shininess) { this->materialData->shininess = shininess; }

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
	/// ライティングの種類のゲッター
	/// </summary>
	/// <returns></returns>
	const int& GetLightingMode() const { return materialData->lightingMode; }

	/// <summary>
	/// ライトの明るさのゲッター
	/// </summary>
	/// <returns></returns>
	const float& GetShininess() const { return materialData->shininess; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 座標変換行列リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> transformationMatrixResource;

	// 頂点リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource;
	// マテリアルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource;

	// 座標変換行列データ
	TransformationMatrix* transformationMatrixData = nullptr;

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
#pragma once
#include "base/DirectXCommon.h"
#include "Data/VertexData.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"

#include <random>
#include <list>

// 変換データ
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// マテリアルデータ
struct Material {
	Vector4 color;
	Matrix4x4 uvTransform;
};

struct ParticleForGPU {
	Matrix4x4 WVP;
	Matrix4x4 world;
	Vector4 color;
};

// パーティクル
struct Particle {
	Transform transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

// エミッター
struct Emitter {
	Transform transform;
	uint32_t count; // 発生数
	float frequency; // 発生頻度
	float frequencyTime; // 頻度用時刻
};

/// ===== カメラ ===== ///
class Camera;

/// ===== パーティクルシステム ===== ///
class ParticleSystem {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::string textureFilePath);

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
	/// パーティクル生成
	/// </summary>
	Particle MakeNewParticle();

	/// <summary>
	/// 生成
	/// </summary>
	std::list<Particle> Emit(const Emitter& emitter);

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
public:

	/// <summary>
	/// パーティクルデータ初期化
	/// </summary>
	void InitializeParticleData();

	/// <summary>
	/// 頂点データ初期化
	/// </summary>
	void InitializeVertexData();

	/// <summary>
	/// 参照データ初期化
	/// </summary>
	void InitializeIndexData();

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

	void SetTexture(std::string textureFilePath) { this->textureFilePath = textureFilePath; }

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera) { this->camera = camera; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// パーティクルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> ParticleResource;

	// 頂点リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource;
	// 参照リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> indexResource;
	// マテリアルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource;

	// 座標変換行列データ
	ParticleForGPU* ParticleData = nullptr;

	// 頂点データ
	VertexData* vertexData = nullptr;
	// 参照データ
	uint32_t* indexData = nullptr;
	// マテリアルデータ
	Material* materialData = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// Index用のビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	// テクスチャファイルパス
	std::string textureFilePath = "";

	// SRVインデックス
	uint32_t srvIndex;

	// カメラ
	Camera* camera = nullptr;

	// インスタンス数
	static const int kNumMaxInstance = 10;
	// 描画すべきインスタンスの数
	uint32_t numInstance = 0;
	
	// パーティクル
	std::list<Particle> particles;

	// Δt
	const float kDeltaTime = 1.0f / 60.0f;

	Matrix4x4 backToFrontMatrix = {};

	// 乱数生成器
	std::random_device seedGenerator;
	std::mt19937 randomEngine;
};
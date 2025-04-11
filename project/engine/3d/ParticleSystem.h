#pragma once
#include "Data/VertexData.h"
#include "Data/Transform.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"

#include <d3d12.h>
#include <random>
#include <list>
#include <unordered_map>
#include <wrl.h>

// マテリアルデータ
struct Material {
	Vector4 color;
	Matrix4x4 uvTransform;
};

// シェーダー用パーティクルデータ
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

// パーティクルグループ
struct ParticleGroup {
	std::string textureFilePath; // テクスチャファイルパス
	std::list<Particle> particles; // パーティクルのリスト
	uint32_t srvIndex; // SRVインデックス
	Microsoft::WRL::ComPtr <ID3D12Resource> particleResource; // パーティクルリソース
	ParticleForGPU* particleData; // パーティクルデータ
	uint32_t numInstance = 0; // インスタンスの数
};

/// ===== 前方宣言 ===== ///
class Camera;
class DirectXUtility;
class SwapChain;

/// ===== パーティクルシステム ===== ///
class ParticleSystem {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static ParticleSystem* instance;
	// コンストラクタの隠蔽
	ParticleSystem() = default;
	// デストラクタの隠蔽
	~ParticleSystem() = default;
	// コピーコンストラクタの封印
	ParticleSystem(ParticleSystem&) = delete;
	// コピー代入演算子の封印
	ParticleSystem& operator=(ParticleSystem&) = delete;

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
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui表示
	/// </summary>
	/// <param name="name"></param>
	void ShowImGui(const char* name);

	/// <summary>
	/// パーティクルグループの生成
	/// </summary>
	/// <param name="name"></param>
	/// <param name="textureFilePath"></param>
	void CreateParticleGroup(const std::string name, const std::string textureFilePath);

	/// <summary>
	/// パーティクルの発生
	/// </summary>
	void Emit(const std::string name, const Vector3& position, uint32_t count);

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
public:

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
	/// パーティクル生成
	/// </summary>
	Particle MakeNewParticle();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static ParticleSystem* GetInstance();

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

	// 頂点リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource;
	// 参照リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> indexResource;
	// マテリアルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource;

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

	// カメラ
	Camera* camera = nullptr;

	// インスタンス数
	static const int kNumMaxInstance = 50;

	// Δt
	const float kDeltaTime = 1.0f / 60.0f;

	Matrix4x4 backToFrontMatrix = {};

	// 乱数生成器
	std::random_device seedGenerator;
	std::mt19937 randomEngine;

	// パーティクルグループコンテナ
	std::unordered_map<std::string, ParticleGroup> particleGroups;
};
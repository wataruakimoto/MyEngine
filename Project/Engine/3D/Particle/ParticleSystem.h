#pragma once
#include "Data/VertexData.h"
#include "Data/Transform.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Particle.h"
#include "NEW/ParticleData.h"

#include <d3d12.h>
#include <random>
#include <list>
#include <numbers>
#include <unordered_map>
#include <wrl.h>

/// ===== 前方宣言 ===== ///
class Camera;
class DirectXUtility;
class BaseParticleType;

// シェーダー用パーティクルデータ
struct ParticleForGPU {
	Matrix4x4 WVP;
	Matrix4x4 world;
	Vector4 color;
};

// パーティクルグループ
struct ParticleGroup {
	std::string textureFilePath; // テクスチャファイルパス
	std::list<Particle> particles; // パーティクルのリスト
	uint32_t srvIndex; // SRVインデックス
	Microsoft::WRL::ComPtr <ID3D12Resource> particleResource; // パーティクルリソース
	ParticleForGPU* particleData; // パーティクルデータ
	uint32_t numInstance = 0; // インスタンスの数
	ParticleShape particleType; // パーティクルの種類
	BaseParticleType* particleTypeClass; // パーティクルの種類クラス
};

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
	/// <param effectName="effectName"></param>
	void ShowImGui(const char* name);

	/// <summary>
	/// パーティクルグループの生成
	/// </summary>
	/// <param effectName="effectName"></param>
	/// <param effectName="textureFilePath"></param>
	void CreateParticleGroup(const std::string name, const std::string textureFilePath, ParticleShape type);

	/// <summary>
	/// パーティクルの発生
	/// </summary>
	void Emit(const std::string name, const Vector3& position, uint32_t count, Particle setting);

	void EmitExplosion(const std::string name, const Vector3& center, float radius, uint32_t count, Particle setting);

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
public:

	/// <summary>
	/// パーティクル生成
	/// </summary>
	Particle MakeNewParticle(Particle setting);

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
	/// <param effectName="camera">カメラ</param>
	void SetCamera(Camera* camera) { this->camera = camera; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

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

	// ワールド行列
	Matrix4x4 worldMatrix = {};

	// WorldViewProjection行列
	Matrix4x4 worldViewProjectionMatrix = {};

	// DirectXUtilityのインスタンス
	DirectXUtility* dxUtility = nullptr;
};
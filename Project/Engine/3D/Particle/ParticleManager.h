#pragma once

#include "ParticleData.h"
#include "ShapeRenderers/PlaneRenderer.h"
#include "ShapeRenderers/RingRenderer.h"
#include "ShapeRenderers/CylinderRenderer.h"
#include "ShapeRenderers/CubeRenderer.h"
#include "ShapeRenderers/ShardRenderer.h"

#include <unordered_map>
#include <string>
#include <list>
#include <memory>

/// ===== 前方宣言 ===== ///
class TextureManager;
class DirectXUtility;
class SrvManager;
class Camera;

/// ===== パ＝ティクルマネージャー ===== ///
class ParticleManager {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static ParticleManager* instance;

	// コンストラクタの隠蔽
	ParticleManager() = default;
	// デストラクタの隠蔽
	~ParticleManager() = default;
	// コピーコンストラクタの無効化
	ParticleManager(const ParticleManager&) = delete;
	// 代入演算子の無効化
	ParticleManager& operator=(const ParticleManager&) = delete;

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
	void ShowImGui();

	/// <summary>
	/// 設定の追加
	/// </summary>
	/// <param effectName="setting">設定</param>
	void AddSetting(ParticleSetting& setting);

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param effectName="instance">インスタンス</param>
	void AddInstance(const ParticleInstance& instance);

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// Groupごとのパーティクル更新
	/// </summary>
	/// <param name="particles"></param>
	void UpdateParticles(std::list<ParticleInstance>& particles);

	/// <summary>
	/// グループコンテナの更新
	/// </summary>
	/// <param name="groups"></param>
	void UpdateGroups(std::unordered_map<std::string, ParticleGroup>& groups);

	/// <summary>
	/// JSONからパーティクル設定を全て読み込み
	/// </summary>
	void LoadParticleSettingsFromJSON();

	/// <summary>
	/// エフェクトのリストを表示
	/// </summary>
	void ShowEffectList();

	/// <summary>
	/// パラメータを表示
	/// </summary>
	void ShowParameters();

	/// <summary>
	/// 設定をJSONに保存
	/// </summary>
	/// <param name="effectName">エフェクト名</param>
	void SaveSettingsToJSON(const std::string& effectName);

	/// <summary>
	/// 設定をJSONから読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadSettingsFromJSON(const std::string& filePath);

	/// <summary>
	/// グループのリソース作成
	/// </summary>
	/// <param name="group"></param>
	void CreateGroupResource(ParticleGroup& group);

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static ParticleManager* GetInstance();

	/// <summary>
	/// 設定のゲッター
	/// </summary>
	/// <param effectName="effectName">エフェクト名</param>
	/// <returns></returns>
	ParticleSetting* GetSetting(const std::string& effectName);

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

	// テクスチャマネージャーのインスタンス
	TextureManager* textureManager;

	// DirectXユーティリティのインスタンス
	DirectXUtility* dxUtility;

	// SRVマネージャーのインスタンス
	SrvManager* srvManager;

	// カメラの借りポインタ
	Camera* camera = nullptr;

	// 設定のコンテナ エフェクト名、設定
	std::unordered_map<std::string, ParticleSetting> settings;

	// グループコンテナ  エフェクト名、グループ
	std::unordered_map<std::string, ParticleGroup> planeGroups;
	std::unordered_map<std::string, ParticleGroup> ringGroups;
	std::unordered_map<std::string, ParticleGroup> cylinderGroups;
	std::unordered_map<std::string, ParticleGroup> cubeGroups;
	std::unordered_map<std::string, ParticleGroup> shardGroups;

	// Δt
	const float kDeltaTime = 1.0f / 60.0f;

	// 板ポリのレンダラー
	std::unique_ptr<PlaneRenderer> planeRenderer = nullptr;

	// リングのレンダラー
	std::unique_ptr<RingRenderer> ringRenderer = nullptr;

	// シリンダーのレンダラー
	std::unique_ptr<CylinderRenderer> cylinderRenderer = nullptr;

	// キューブのレンダラー
	std::unique_ptr<CubeRenderer> cubeRenderer = nullptr;

	// シャードのレンダラー
	std::unique_ptr<ShardRenderer> shardRenderer = nullptr;

	// Jsonデータを入れておくフォルダパス
	const std::string DataFolderPath = "Resources/Datas/Particles/";

	// 画像データを入れておくフォルダパス
	const std::string TextureFolderPath = "Resources/Textures/Particles/";

	// エディタで選択中のエフェクト名
	std::string currentEditName = "";

	// 新規作成時の入力バッファ
	char inputNameBuffer[64] = ""; // 最大64文字

	// 編集用の一時設定
	ParticleSetting tempSetting;

	// ViewProjection行列
	Matrix4x4 viewProjectionMatrix = {};

	// ビルボード行列
	Matrix4x4 billboardMatrix = {};
};


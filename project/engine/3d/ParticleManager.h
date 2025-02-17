#pragma once

/// === パーティクル管理 === ///
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
	// コピーコンストラクタの封印
	ParticleManager(ParticleManager&) = delete;
	// コピー代入演算子の封印
	ParticleManager& operator=(ParticleManager&) = delete;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static ParticleManager* GetInstance();

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	//struct ParticleGroup {
	//	MaterialData material; // マテリアルデータ
	//	std::list<Particle> particles; // パーティクルのリスト
	//	// インスタンシングデータ用SRVインデックス
	//	// インスタンシングリソース
	//	uint32_t numInstance; // インスタンス数
	//	// インスタンシングデータを書き込むためのポインタ
	//};

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:
};
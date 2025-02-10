#pragma once
#include "base/DirectXCommon.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

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

	// 変換データ
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	struct Particle {
		Transform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};

	// 頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	// マテリアルデータ
	struct MaterialData {
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};

	// モデルデータ
	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};

	struct ParticleGroup {
		MaterialData material; // マテリアルデータ
		std::list<Particle> particles; // パーティクルのリスト
		// インスタンシングデータ用SRVインデックス
		// インスタンシングリソース
		uint32_t numInstance; // インスタンス数
		// インスタンシングデータを書き込むためのポインタ
	};

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// RootSignatureの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// InputLayoutの生成
	/// </summary>
	void CreateInputLayout();

	/// <summary>
	/// BlendStateの生成
	/// </summary>
	void CreateBlendState();

	/// <summary>
	/// RasterizerStateの生成
	/// </summary>
	void CreateRasterizerState();

	/// <summary>
	/// VertexShaderの生成
	/// </summary>
	void CreateVertexShader();

	/// <summary>
	/// PixelShaderの生成
	/// </summary>
	void CreatePixelShader();

	/// <summary>
	/// DepthStencilStateの生成
	/// </summary>
	void CreateDepthStencilState();

	/// <summary>
	/// GraphicsPipelineの生成
	/// </summary>
	void CreateGraphicsPipeline();

	/// <summary>
	/// 頂点データ初期化
	/// </summary>
	void InitializeVertexData();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// DirectXCommonのポインタ
	DirectXCommon* dxCommon_ = nullptr;

	// RootSignature
	Microsoft::WRL::ComPtr <ID3D12RootSignature> rootSignature = nullptr;

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};

	// BlendState
	D3D12_BLEND_DESC blendDesc{};

	// RasterizerState
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	// VertexShader
	Microsoft::WRL::ComPtr <IDxcBlob> vertexShaderBlob = nullptr;

	// PixelShader
	Microsoft::WRL::ComPtr <IDxcBlob> pixelShaderBlob = nullptr;

	// DepthStencilState
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	// GraphicsPipeline
	Microsoft::WRL::ComPtr <ID3D12PipelineState> graphicsPipelineState = nullptr;

	// 頂点リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	// 頂点データ
	VertexData* vertexData = nullptr;

	// objファイルのデータ
	ModelData modelData;
};
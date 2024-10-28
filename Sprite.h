#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "DirectXCommon.h"

// 頂点データ
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
};

// 座標変換行列
struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 world;
};

// マテリアルデータ
struct Material {
	Vector4 color;
	float padding[3];
};

// 
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

/// === スプライト共通部の前方宣言 === ///
class SpriteCommon;

// スプライト
class Sprite {

///=====================================================/// 
/// メンバ関数
///=====================================================///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SpriteCommon* spriteCommon);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

///=====================================================/// 
/// メンバ変数 
///=====================================================///
private:

	SpriteCommon* spriteCommon_ = nullptr;

	// 頂点リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource;
	// リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> indexResource;
	// 座標変換行列リソース
	Microsoft::WRL::ComPtr <ID3D12Resource> transformationMatrixResource;
	// マテリアルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource;

	// 
	VertexData* vertexData = nullptr;
	// 
	uint32_t* indexData = nullptr;
	// 
	TransformationMatrix* transformationMatrixData = nullptr;
	// 
	Material* materialData = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// Index用のビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
};
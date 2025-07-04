#pragma once
#include "3D/Data/VertexData.h"
#include "3D/Data/TransformationData.h"

#include <d3d12.h>
#include <wrl.h>
#include <stdint.h>

/// === スカイボックス === ///
class Skybox {

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
	/// 頂点データの初期化
	/// </summary>
	void InitializeVertexData();

	/// <summary>
	/// 座標変換データの初期化
	/// </summary>
	void InitializeTransformationData();

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

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	// 頂点データ
	VertexData* vertexData = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	// インデックスデータ
	uint32_t* indexData = nullptr;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView = {};

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource;
	// 座標変換データ
	TransformationData* transformationData = nullptr;
};


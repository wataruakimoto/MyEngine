#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "DirectXCommon.h"

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
	void Initialize(SpriteCommon* spriteCommon, std::string textureFilePath);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

///=====================================================/// 
/// クラス内関数
///=====================================================///
private:

	/// <summary>
	/// 頂点データ初期化
	/// </summary>
	void InitializeVertexData();

	/// <summary>
	/// 座標変換行列データ初期化
	/// </summary>
	void InitializeTransformationMatrixData();

	/// <summary>
	/// マテリアルデータ初期化
	/// </summary>
	void InitializeMaterialData();

///=====================================================/// 
/// ゲッター&セッター
///=====================================================///
public:

	/// <summary>
	/// サイズのゲッター
	/// </summary>
	/// <returns>size</returns>
	const Vector2& GetSize()const { return size; }

	/// <summary>
	/// サイズのセッター
	/// </summary>
	/// <param name="size">size</param>
	void SetSize(const Vector2& size) { this->size = size; }

	/// <summary>
	/// 回転のゲッター
	/// </summary>
	/// <returns>rotation</returns>
	float GetRotation() const { return rotation; }
	
	/// <summary>
	/// 回転のセッター
	/// </summary>
	/// <param name="rotation">rotation</param>
	void SetRotation(float rotation) { this->rotation = rotation; }

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	/// <returns>position</returns>
	const Vector2& GetPosition()const { return position; }

	/// <summary>
	/// 座標のセッター
	/// </summary>
	/// <param name="position">position</param>
	void SetPosition(const Vector2& position) { this->position = position; }

	/// <summary>
	/// 色のゲッター
	/// </summary>
	/// <returns>color</returns>
	const Vector4& GetColor()const { return materialData->color; }

	/// <summary>
	/// 色のセッター
	/// </summary>
	/// <param name="color">color</param>
	void SetColor(const Vector4& color) { materialData->color = color; }

///=====================================================/// 
/// 構造体
///=====================================================///
public:

	// 変換データ
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	// 頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};

	// 座標変換行列
	struct TransformationMatrix {
		Matrix4x4 WVP;
	};

	// マテリアルデータ
	struct Material {
		Vector4 color;
	};

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

	// 頂点データ
	VertexData* vertexData = nullptr;
	// 参照データ
	uint32_t* indexData = nullptr;
	// 座標変換行列データ
	TransformationMatrix* transformationMatrixData = nullptr;
	// マテリアルデータ
	Material* materialData = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// Index用のビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	// サイズ
	Vector2 size = { 256.0f,256.0f };

	// 回転
	float rotation = 0.0f;

	// 座標
	Vector2 position = { 0.0f,0.0f };
	
	// テクスチャ番号
	uint32_t textureIndex = 0;
};
#include "Sprite.h"
#include "SpriteCommon.h"
#include "math/MathMatrix.h"
#include "winApp/WinApp.h"
#include "TextureManager.h"

using namespace MathMatrix;

void Sprite::Initialize(SpriteCommon* spriteCommon, std::string textureFilePath) {

	// 引数をメンバ変数に代入
	this->spriteCommon_ = spriteCommon;

	InitializeVertexData();

	InitializeTransformationMatrixData();

	InitializeMaterialData();

	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);

	AdjustTextureSize();
}

void Sprite::Update() {

	/// === アンカーポイント反映 === ///

	float left = 0.0f - anchorPoint.x;
	float right = 1.0f - anchorPoint.x;
	float top = 0.0f - anchorPoint.y;
	float bottom = 1.0f - anchorPoint.y;

	/// === フリップ反映 === ///

	// 左右反転
	if (isFlipX) {
		left = -left;
		right = -right;
	}

	// 上下反転
	if (isFlipY) {
		top = -top;
		bottom = -bottom;
	}

	/// === テクスチャ範囲反映 === ///

	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetadata(textureIndex);

	float texLeft = textureLeftTop.x / metadata.width;
	float texRight = (textureLeftTop.x + textureSize.x) / metadata.width;
	float texTop = textureLeftTop.y / metadata.height;
	float texBottom = (textureLeftTop.y + textureSize.y) / metadata.height;

	/// === VertexResourceにデータを書き込む(4頂点) === ///
	
	// 左下
	vertexData[0].position = { left, bottom, 0.0f, 1.0f };
	vertexData[0].texcoord = { texLeft, texBottom };
	// 左上
	vertexData[1].position = { left, top, 0.0f, 1.0f };
	vertexData[1].texcoord = { texLeft, texTop };
	// 右下
	vertexData[2].position = { right, bottom, 0.0f, 1.0f };
	vertexData[2].texcoord = { texRight, texBottom };
	// 右上
	vertexData[3].position = { right, top, 0.0f, 1.0f };
	vertexData[3].texcoord = { texRight, texTop };

	/// === IndexResourceにデータを書き込む(6個分)=== ///

	indexData[0] = 0; // 左下
	indexData[1] = 1; // 左上
	indexData[2] = 2; // 右下
	indexData[3] = 1; // 左上
	indexData[4] = 3; // 右上
	indexData[5] = 2; // 右下

	/// === Transform情報を作る === ///
	Transform transform{ {size.x, size.y, 1.0f}, {0.0f, 0.0f, rotation}, {position.x, position.y, 0.0f} };
	
	/// === TransformからWorldMatrixを作る === ///
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	/// === ViewMatrixを作って単位行列を代入 === ///
	Matrix4x4 viewMatrix = MakeIdentity4x4();

	/// === ProjectionMatrixを作って平行投影行列を書き込む === ///
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);

	transformationMatrixData->WVP = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
}

void Sprite::Draw() {

	/// === VertexBufferViewを設定 === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	/// === IndexBufferViewを設定 === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	/// === 座標変換行列CBufferの場所を設定 === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, transformationMatrixResource->GetGPUVirtualAddress());

	/// === マテリアルCBufferの場所を設定 === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, materialResource->GetGPUVirtualAddress());

	/// === SRVのDescriptorTableの先頭を設定 === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVGPUHandle(textureIndex));

	/// === 描画(DrawCall) === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::InitializeVertexData() {

	/// === VertexResourceを作る === ///
	vertexResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 6);

	/// === IndexResourceを作る === ///
	indexResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	/// === VBVを作成する(値を設定するだけ) === ///

	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	// 1頂点あたりのサイズ　
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	/// === IBVを作成する(値を設定するだけ) === ///

	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	/// === VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる === ///
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	/// === IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる === ///
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
}

void Sprite::InitializeTransformationMatrixData() {

	/// === TransformationMatrixResourceを作る === ///
	transformationMatrixResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	/// === TransformationMatrixResourceにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる === ///
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	/// === TransformationMatrixDataの初期値を書き込む === ///
	transformationMatrixData->WVP = MakeIdentity4x4(); // 単位行列を書き込む
}

void Sprite::InitializeMaterialData() {

	/// === MaterialResourceを作る === ///
	materialResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	/// === MaterialResourceにデータを書き込むためのアドレスを取得してmaterialDataに割り当てる === ///
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	/// === MaterialDataの初期値を書き込む === ///
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 白を書き込む
}

void Sprite::AdjustTextureSize() {

	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetadata(textureIndex);

	textureSize.x = static_cast<float>(metadata.width);
	textureSize.y = static_cast<float>(metadata.height);

	// 画像サイズをテクスチャサイズにあわせる
	size = textureSize;
}
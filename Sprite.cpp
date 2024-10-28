#include "Sprite.h"
#include "SpriteCommon.h"
#include "MathMatrix.h"
#include "WinApp.h"

using namespace MathMatrix;

void Sprite::Initialize(SpriteCommon* spriteCommon) {

	// 引数をメンバ変数に代入
	this->spriteCommon_ = spriteCommon;

	// VertexResourceを作る
	vertexResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 6);

	// IndexResourceを作る
	indexResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	// TransformationMatrixResourceを作る
	transformationMatrixResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	// MaterialResourceを作る
	materialResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	// VBVを作成する(値を設定するだけ)
	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	// 1頂点あたりのサイズ　
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// IBVを作成する(値を設定するだけ)
	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// VertexResourceにデータを書き込むためのアドレスを取得してVertexDataに割り当てる
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	// TransformationMatrixResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	// MaterialResourceにデータを書き込むためのアドレスを取得してmaterialDataに割り当てる
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	// TransformationMatrixDataに単位行列を書き込んでおく
	transformationMatrixData->WVP = MakeIdentity4x4();
	transformationMatrixData->world = MathMatrix::MakeIdentity4x4();

	// MaterialDataの初期値を書き込む
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 白を書き込み
}

void Sprite::Update() {

	/// === 頂点リソースにデータを書き込む(4頂点) === ///
	
	// 1枚目の三角形
	vertexData[0].position = { 0.0f, 360.0f, 0.0f, 1.0f }; // 左下
	vertexData[0].texcoord = { 0.0f, 1.0f };
	vertexData[1].position = { 0.0f, 0.0f, 0.0f, 1.0f }; // 左上
	vertexData[1].texcoord = { 0.0f, 0.0f };
	vertexData[2].position = { 640.0f, 360.0f, 0.0f, 1.0f }; // 右下
	vertexData[2].texcoord = { 1.0f, 1.0f };

	// 2枚目の三角形
	vertexData[3].position = { 0.0f, 0.0f, 0.0f, 1.0f }; // 左上
	vertexData[3].texcoord = { 0.0f, 0.0f };
	vertexData[4].position = { 640.0f, 0.0f, 0.0f, 1.0f }; // 右上
	vertexData[4].texcoord = { 1.0f, 0.0f };
	vertexData[5].position = { 640.0f, 360.0f, 0.0f, 1.0f }; // 右下
	vertexData[5].texcoord = { 1.0f, 1.0f };

	/// === インデックスリソースにデータを書き込む(6個分)=== ///

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 1;
	indexData[4] = 4;
	indexData[5] = 2;

	/// === Transform情報を作る === ///
	Transform transform{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	/// === TransformからWorldMatrixを作る === ///
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	/// === ViewMatrixを作って単位行列を代入 === ///
	Matrix4x4 viewMatrix = MakeIdentity4x4();

	/// === ProjectionMatrixを作って平行投影行列を書き込む === ///
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);

	transformationMatrixData->WVP = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->world = worldMatrix;
}

void Sprite::Draw() {

	/// === VertexBufferViewを設定 === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	/// === IndexBufferViewを設定 === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	/// === 座標変換行列CBufferの場所を設定 === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	/// === マテリアルCBufferの場所を設定 === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	/// === SRVのDescriptorTableの先頭を設定 === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, spriteCommon_->GetDxCommon()->GetSRVGPUDescriptorHandle(1));

	/// === 描画(DrawCall) === ///
	spriteCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
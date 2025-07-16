#include "Skybox.h"
#include "SkyboxCommon.h"
#include "base/DirectXUtility.h"
#include "2D/TextureManager.h"
#include "Math/MathMatrix.h"
#include "camera/Camera.h"

#include <imgui.h>

using namespace MathMatrix;

void Skybox::Initialize(std::string directoryPath, std::string fileName) {

	// 引数からファイルパスを作成
	filePath = directoryPath + fileName;

	// テクスチャを読み込む
	TextureManager::GetInstance()->LoadTexture(filePath);

	// 頂点データの初期化
	InitializeVertexData();

	// インデックスデータの初期化
	InitializeIndexData();

	// 座標変換データの初期化
	InitializeTransformationData();

	// マテリアルデータの初期化
	InitializeMaterialData();
}

void Skybox::Update() {

	// 変換データからワールド行列を作成
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	// カメラからビュープロジェクション行列を取得
	Matrix4x4 viewProjectionMatrix = camera->GetViewProjectionMatrix();

	// ワールド行列とビュープロジェクション行列を掛け合わせてWVP行列を作成
	transformationData->WVP = worldMatrix * viewProjectionMatrix;
}

void Skybox::Draw() {

	// 頂点バッファビューを設定
	SkyboxCommon::GetInstance()->GetdxUtility()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// インデックスバッファビューを設定
	SkyboxCommon::GetInstance()->GetdxUtility()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// 座標変換リソースのCBufferの場所を設定
	SkyboxCommon::GetInstance()->GetdxUtility()->GetCommandList()->SetGraphicsRootConstantBufferView(0, transformationResource->GetGPUVirtualAddress());

	// マテリアルリソースのCBufferの場所を設定
	SkyboxCommon::GetInstance()->GetdxUtility()->GetCommandList()->SetGraphicsRootConstantBufferView(1, materialResource->GetGPUVirtualAddress());

	// SRVのDescriptorTableの場所を設定
	SkyboxCommon::GetInstance()->GetdxUtility()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVGPUHandle(filePath));

	// 描画(DrawCall)
	SkyboxCommon::GetInstance()->GetdxUtility()->GetCommandList()->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
}

void Skybox::ShowImGui(const char* name) {

	ImGui::Begin(name);
	// スカイボックスのファイルパスを表示
	ImGui::Text("FilePath: %s", filePath.c_str());
	ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f);
	ImGui::SliderAngle("Rotate X", &transform.rotate.x, -180.0f, 180.0f);
	ImGui::SliderAngle("Rotate Y", &transform.rotate.y, -180.0f, 180.0f);
	ImGui::SliderAngle("Rotate Z", &transform.rotate.z, -180.0f, 180.0f);
	ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f);
	ImGui::End();
}

void Skybox::InitializeVertexData() {

	// 頂点リソースを作成
	vertexResource = SkyboxCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(VertexData) * vertexCount);

	/// === 頂点バッファビューを作成 === ///

	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点数分
	vertexBufferView.SizeInBytes = sizeof(VertexData) * vertexCount;
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// 頂点リソースに頂点データを書き込むためのアドレスを取得、割り当てする
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	/// === 頂点リソースに初期値を書き込む === ///

	vertexData[0].position = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 右上前
	vertexData[1].position = Vector4(1.0f, 1.0f, -1.0f, 1.0f); // 右上後
	vertexData[2].position = Vector4(1.0f, -1.0f, 1.0f, 1.0f); // 右下前
	vertexData[3].position = Vector4(1.0f, -1.0f, -1.0f, 1.0f); // 右下後
	vertexData[4].position = Vector4(-1.0f, 1.0f, 1.0f, 1.0f); // 左上前
	vertexData[5].position = Vector4(-1.0f, 1.0f, -1.0f, 1.0f); // 左上後
	vertexData[6].position = Vector4(-1.0f, -1.0f, 1.0f, 1.0f); // 左下前
	vertexData[7].position = Vector4(-1.0f, -1.0f, -1.0f, 1.0f); // 左下後
}

void Skybox::InitializeIndexData() {

	// インデックスリソースを作成
	indexResource = SkyboxCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(uint32_t) * indexCount);

	/// === インデックスバッファビューを作成 === ///

	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデックス数分
	indexBufferView.SizeInBytes = sizeof(uint32_t) * indexCount;
	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// インデックスリソースにインデックスデータを書き込むためのアドレスを取得、割り当てする
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	/// === インデックスリソースに初期値を書き込む === ///

	// 右 +X
	indexData[0] = 0; // 右上前
	indexData[1] = 1; // 右上後
	indexData[2] = 2; // 右下前
	indexData[3] = 2; // 右下前
	indexData[4] = 1; // 右上後
	indexData[5] = 3; // 右下後

	// 左 -X
	indexData[6] = 4; // 左上前
	indexData[7] = 5; // 左上後
	indexData[8] = 6; // 左下前
	indexData[9] = 6; // 左下前
	indexData[10] = 5; // 左上後
	indexData[11] = 7; // 左下後

	// 上 +Y
	indexData[12] = 0; // 右上前
	indexData[13] = 4; // 左上前
	indexData[14] = 1; // 右上後
	indexData[15] = 1; // 右上後
	indexData[16] = 4; // 左上前
	indexData[17] = 5; // 左上後

	// 下 -Y
	indexData[18] = 2; // 右下前
	indexData[19] = 6; // 左下前
	indexData[20] = 3; // 右下後
	indexData[21] = 3; // 右下後
	indexData[22] = 6; // 左下前
	indexData[23] = 7; // 左下後

	// 前 +Z
	indexData[24] = 0; // 右上前
	indexData[25] = 2; // 右下前
	indexData[26] = 4; // 左上前
	indexData[27] = 4; // 左上前
	indexData[28] = 2; // 右下前
	indexData[29] = 6; // 左下前

	// 後 -Z
	indexData[30] = 1; // 右上後
	indexData[31] = 3; // 右下後
	indexData[32] = 5; // 左上後
	indexData[33] = 5; // 左上後
	indexData[34] = 3; // 右下後
	indexData[35] = 7; // 左下後
}

void Skybox::InitializeTransformationData() {

	// 座標変換リソースを作成
	transformationResource = SkyboxCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(TransformationData));

	// 座標変換リソースに座標変換データを書き込むためのアドレスを取得、割り当てする
	transformationResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));

	// 座標変換データの初期値を書き込む
	transformationData->WVP = MakeIdentity4x4(); // 単位行列を書き込む
}

void Skybox::InitializeMaterialData() {

	// マテリアルリソースを作成
	materialResource = SkyboxCommon::GetInstance()->GetdxUtility()->CreateBufferResource(sizeof(Material));

	// マテリアルリソースにマテリアルデータを書き込むためのアドレスを取得、割り当てする
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	// マテリアルデータの初期値を書き込む
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 白色
}

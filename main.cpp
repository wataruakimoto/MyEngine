#include <dxgidebug.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "MathVector.h"
#include "MathMatrix.h"
#include <cmath>
#include <fstream>
#include <sstream>

#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"

#pragma comment(lib,"dxcompiler.lib")

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "D3DResourceLeakChecker.h"
#include "Logger.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3dCommon.h"
#include "Object3d.h"
#include "TextureManager.h"

using namespace MathMatrix;

//truct Transform {
//	Vector3 scale;
//	Vector3 rotate;
//	Vector3 translate;
//;
//
//truct VertexData {
//	Vector4 position;
//	Vector2 texcoord;
//	Vector3 normal;
//;
//
//truct Material {
//	Vector4 color;
//	bool enableLighting;
//	float padding[3];
//	Matrix4x4 uvTransform;
//;
//
//truct TransformationMatrix {
//	Matrix4x4 WVP;
//	Matrix4x4 World;
//;
//
//truct DirectionalLight {
//	Vector4 color; // !< ライトの色
//	Vector3 direction; // !< ライトの向き
//	float intensity; // !< 輝度
//;
//
//truct MaterialData {
//	std::string textureFilePath;
//;
//
//truct ModelData {
//	std::vector<VertexData> vertices;
//	MaterialData material;
//;
//
/// mtlファイルを読み込む関数
//aterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
//
//	// 1. 中で必要となる変数の宣言
//	MaterialData materialData; // 構築するMaterialData
//	std::string line; // ファイルから読んだ1行を格納するもの
//
//	// 2. ファイルを開く
//	std::ifstream file(directoryPath + "/" + filename); //ファイルを開く
//	assert(file.is_open()); // とりあえず開けなかったら止める
//
//	// 3. 実際にファイルを読み、MaterialDataを構築していく
//	while (std::getline(file, line)) {
//		std::string identifier;
//		std::istringstream s(line);
//		s >> identifier;
//
//		// identifierに応じた処理
//		if (identifier == "map_Kd") {
//
//			std::string textureFilename;
//			s >> textureFilename;
//			// 連結してファイルパスにする
//			materialData.textureFilePath = directoryPath + "/" + textureFilename;
//		}
//	}
//
//	// 4. MaterialDataを返す
//	return materialData;
//
//
/// objファイルを読み込む関数
//odelData LoadObjFile(const std::string& directoryPath, const std::string& filename) {
//
//	// 1. 中で必要となる変数の宣言
//	ModelData modelData; // 構築するModelData
//	std::vector<Vector4>positions; // 位置
//	std::vector<Vector3>normals; // 法線
//	std::vector<Vector2>texcoords; // テクスチャ座標
//	std::string line; // ファイルから読んだ１行を格納するもの
//
//	// 2. ファイルを開く
//	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
//	assert(file.is_open()); // とりあえず開けなかったら止める
//
//	// 3. 実際にファイルを読み、ModelDataを構築していく
//	while (std::getline(file, line)) {
//		std::string identifier;
//		std::istringstream s(line);
//		s >> identifier; // 先頭の識別子を読む
//
//		// identifierに応じた処理
//		if (identifier == "v") {
//
//			Vector4 position;
//			s >> position.x >> position.y >> position.z;
//			position.w = 1.0f;
//			positions.push_back(position);
//
//		} else if (identifier == "vt") {
//
//			Vector2 texcoord;
//			s >> texcoord.x >> texcoord.y;
//			texcoords.push_back(texcoord);
//
//		} else if (identifier == "vn") {
//
//			Vector3 normal;
//			s >> normal.x >> normal.y >> normal.z;
//			normals.push_back(normal);
//
//		} else if (identifier == "f") {
//
//			VertexData triangle[3];
//
//			// 面は三角形限定。その他は未対応
//			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
//				std::string vertexDefinition;
//				s >> vertexDefinition;
//				// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
//				std::istringstream v(vertexDefinition);
//				uint32_t elementIndices[3];
//				for (int32_t element = 0; element < 3; ++element) {
//					std::string index;
//					std::getline(v, index, '/'); // /区切りでインデックスを読んでいく
//					elementIndices[element] = std::stoi(index);
//				}
//				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
//				Vector4 position = positions[elementIndices[0] - 1];
//				Vector2 texcoord = texcoords[elementIndices[1] - 1];
//				Vector3 normal = normals[elementIndices[2] - 1];
//
//				position.x *= -1.0f; // 位置xを反転
//				texcoord.y = 1.0f - texcoord.y; // 原点を左下から左上に変更
//				normal.x *= -1.0f; // 法線xも反転
//
//				//VertexData vertex = { position,texcoord,normal };
//				//modelData.vertices.push_back(vertex);
//				triangle[faceVertex] = { position,texcoord,normal };
//			}
//
//			// 頂点を逆順で登録することで周り順を逆にする
//			modelData.vertices.push_back(triangle[2]);
//			modelData.vertices.push_back(triangle[1]);
//			modelData.vertices.push_back(triangle[0]);
//
//		} else if (identifier == "mtllib") {
//
//			// materialTemplateLibraryファイルの名前を取得する
//			std::string materialFilename;
//			s >> materialFilename;
//			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
//			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
//		}
//	}
//
//	// 4. ModelDataを返す
//	return modelData;
//

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	///
	/// 初期化処理開始
	///

	/// ----------ポインタ置き場----------

	// 入力のポインタ
	Input* input = nullptr;

	// WindowsAPIのポインタ
	WinApp* winApp = nullptr;

	// DirectX基盤のポインタ
	DirectXCommon* dxCommon = nullptr;

	// スプライト共通部のポインタ
	SpriteCommon* spriteCommon = nullptr;

	// 3Dオブジェクト共通部のポインタ
	Object3dCommon* object3dCommon = nullptr;

	/// ----------ゲームウィンドウ作成----------

	// WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	// ウィンドウを表示する
	ShowWindow(winApp->GetHwnd(), SW_SHOW);

	/// ----------DirectX初期化----------

	// DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	/// ----------汎用機能初期化----------

	// 入力の初期化
	input = new Input();
	input->Initialize(winApp);

	// スプライト共通部初期化
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);

	// 3Dオブジェクト共通部初期化
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);

	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Initialize(dxCommon);

	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	/// ----------シーンの初期化----------

	// スプライトの生成・初期化
	Sprite* sprite = new Sprite();
	sprite->Initialize(spriteCommon, "resources/uvChecker.png");

	// 3Dオブジェクトの生成・初期化
	Object3d* object3d = new Object3d();
	object3d->Initialize();
	
	//// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	//Microsoft::WRL::ComPtr <ID3D12Resource> wvpResource = dxCommon->CreateBufferResource(sizeof(TransformationMatrix));
	//// データを書き込む
	//TransformationMatrix* wvpData = nullptr;
	//// 書き込むためのアドレスを取得
	//wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//// 単位行列を書き込んでおく
	//wvpData->WVP = MakeIdentity4x4();
	//wvpData->world = MakeIdentity4x4();
	//
	//// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	//Microsoft::WRL::ComPtr <ID3D12Resource> materialResource = dxCommon->CreateBufferResource(sizeof(Material));
	//// マテリアルにデータを書き込む
	//Material* materialData = nullptr;
	//// 書き込むためのアドレスを取得
	//materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//// 今は白を書き込んでいる
	//materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//// Lightingを有効にする
	//materialData->enableLighting = true;
	//// 単位行列で初期化
	//materialData->uvTransform = MakeIdentity4x4();
	//
	//// 平行光源用のリソース DirectionalLight
	//Microsoft::WRL::ComPtr <ID3D12Resource> directionalLightResource = dxCommon->CreateBufferResource(sizeof(DirectionalLight));
	//// データを書き込む
	//DirectionalLight* directionalLightData = nullptr;
	//// 書き込むためのアドレスを取得
	//directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	//// デフォルト値は以下のようにしておく
	//directionalLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	//directionalLightData->direction = { 0.0f, -1.0f, 0.0f };
	//directionalLightData->intensity = 1.0f;
	//
	//// モデル読み込み
	//ModelData modelData = LoadObjFile("resources", "axis.obj");
	//// VertexResourceを生成
	//Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource = dxCommon->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	//// 頂点バッファビューを作成する
	//D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//// リソースの先頭アドレスから使う
	//vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//// 使用するリソースのサイズ 頂点のサイズ
	//vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	//// 1頂点あたりのサイズ
	//vertexBufferView.StrideInBytes = sizeof(VertexData);
	//// 頂点リソースにデータを書き込む
	//VertexData* vertexData = nullptr;
	//// 書き込むためのアドレスを取得
	//vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//// 頂点データにリソースをコピー
	//std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
	
	//// Textureを読んで転送する
	//DirectX::ScratchImage mipImages = dxCommon->LoadTexture("resources/uvChecker.png");
	//const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//Microsoft::WRL::ComPtr <ID3D12Resource> textureResource = dxCommon->CreateTextureResource(metadata);
	//dxCommon->UploadTextureData(textureResource, mipImages);
	//
	//// metaDataを基にSRVの設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//srvDesc.Format = metadata.format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	//srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	//
	//D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = dxCommon->GetSRVCPUDescriptorHandle(1);
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = dxCommon->GetSRVGPUDescriptorHandle(1);
	//
	////D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = dxCommon->GetSRVCPUDescriptorHandle(1);
	////D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = dxCommon->GetSRVGPUDescriptorHandle(1);
	//
	//// SRVの生成
	//dxCommon->GetDevice()->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);
	
	//// 2枚目のTextureを読んで転送する
	//DirectX::ScratchImage mipImages2 = dxCommon->LoadTexture(modelData.material.textureFilePath);
	//const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	//Microsoft::WRL::ComPtr <ID3D12Resource> textureResource2 = dxCommon->CreateTextureResource(metadata2);
	//dxCommon->UploadTextureData(textureResource2, mipImages2);
	//
	//// metaData2を基にSRVの設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	//srvDesc2.Format = metadata2.format;
	//srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	//srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);
	//
	//// SRVの生成
	//dxCommon->GetDevice()->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);
	
	//// Transform変数を作る
	//Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//
	//Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	//
	//// UVTransform用の変数
	//Transform uvTransformSprite{
	//	{1.0f,1.0f,1.0f},
	//	{0.0f,0.0f,0.0f},
	//	{0.0f,0.0f,0.0f},
	//};
	//
	//// ImGuiのチェックボックスで切り替えるようにするため
	//bool useMonsterBall = true;

	///
	/// 初期化処終了
	///

	// ウィンドウの×ボタンが押されるまでループ
	while (true) {

		///
		/// メインループ開始
		///

		/// ----------Windowsメッセージ処理----------

		// Windowにメッセージが来てたら最優先で処理させる
		if (winApp->ProcessMessage()) {
			
			// ゲームループを抜ける
			break;

		} else {

			///
			/// 更新処理開始
			///

			/// ----------入力の更新----------
			input->Update();

			//if (input->PushKey(DIK_UP)) {
			//	transform.translate.y += 0.1f;
			//}
			//
			//if (input->TriggerKey(DIK_DOWN)) {
			//	transform.translate.y += -0.1f;
			//}
			//
			//if (input->PushKey(DIK_RIGHT)) {
			//	transform.translate.x += 0.1f;
			//}
			//
			//if (input->TriggerKey(DIK_LEFT)) {
			//	transform.translate.x += -0.1f;
			//}

			/// ----------シーンの更新----------

			/// === ImGui開始 === ///

			// ImGuiにフレーム開始を告げる
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			/// === スプライト更新 === ///
			
			ImGui::Begin("Sprite");
			
			// サイズ変更の確認
			Vector2 size = sprite->GetSize();
			ImGui::DragFloat2("Size", &size.x, 1.0f);
			sprite->SetSize(size);

			// 回転変更の確認
			float rotation = sprite->GetRotation();
			ImGui::DragFloat("Rotation", &rotation, 0.01f);
			sprite->SetRotation(rotation);

			// 座標変更の確認
			Vector2 position = sprite->GetPosition();
			ImGui::DragFloat2("Position", &position.x, 1.0f);
			sprite->SetPosition(position);

			// 色変更の確認
			Vector4 color = sprite->GetColor();
			ImGui::DragFloat4("Color", &color.x, 0.01f);
			sprite->SetColor(color);

			// アンカー変更の確認
			Vector2 anchorPoint = sprite->GetAnchorPoint();
			ImGui::SliderFloat2("Anchor", &anchorPoint.x, -1.0f, 1.0f);
			sprite->SetAnchorPoint(anchorPoint);

			// フリップ変更の確認
			bool isFlipX = sprite->GetIsFlipX();
			bool isFlipY = sprite->GetIsFlipY();
			ImGui::Checkbox("IsFlipX", &isFlipX);
			ImGui::Checkbox("IsFlipY", &isFlipY);
			sprite->SetIsFlipX(isFlipX);
			sprite->SetIsFlipY(isFlipY);

			// テクスチャ左上座標の確認
			Vector2 textureLeftTop = sprite->GetTextureLeftTop();
			ImGui::DragFloat2("TextureLeftTop", &textureLeftTop.x, 1.0f);
			sprite->SetTextureLeftTop(textureLeftTop);

			// テクスチャ切り出しサイズの確認
			Vector2 textureSize = sprite->GetTextureSize();
			ImGui::DragFloat2("TextureSize", &textureSize.x, 1.0f);
			sprite->SetTextureSize(textureSize);

			sprite->Update();
			
			ImGui::End();

			/// === 3Dオブジェクト更新 === ///

			object3d->Update();

			//// TransformからWorldMatrix作成
			//Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
			//Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
			//Matrix4x4 viewMatrix = Inverse(cameraMatrix);
			//Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(winApp->kClientWidth) / float(winApp->kClientHeight), 0.1f, 100.0f);
			//// wvpMatrixを作る
			//Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
			//// CBufferの中身を更新
			//wvpData->WVP = worldViewProjectionMatrix;
			//wvpData->world = worldMatrix;
			
			// ImGuiの内部コマンドを生成する
			ImGui::Render();

			/// === ImGui終了 === ///

			///
			/// 更新処理終了
			///

			///
			/// 描画処理開始
			///

			/// ----------DirectX描画開始----------
			dxCommon->PreDraw();

			/// === スプライトの描画準備 === ///
			spriteCommon->SettingCommonDrawing();

			//TODO: 全てのスプライト個々の描画
			sprite->Draw();

			/// === 3Dオブジェクトの描画準備 === ///
			object3dCommon->SettingCommonDrawing();

			//TODO: 全ての3Dオブジェクト個々の描画
			object3d->Draw();

			/// ----------シーンの描画----------

			//// ここから描画
			//// RootSignatureを設定、PSOに設定しているけど別途設定が必要
			//dxCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
			//dxCommon->GetCommandList()->SetPipelineState(graphicsPipelineState.Get()); // PSOを設定
			//dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView); // VBVを設定
			//// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
			//dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//
			//// 描画用のDescriptorHeapの設定
			//ID3D12DescriptorHeap* descriptorHeaps[] = { dxCommon->GetSRVDescriptorHeap().Get()};
			//dxCommon->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
			//
			//// マテリアルCBufferの場所を設定
			//dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
			//// wvp用のCBufferの場所を設定
			//dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
			//// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
			//dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, useMonsterBall ? textureSrvHandleGPU2 : textureSrvHandleGPU);
			//// 平行光源用
			//dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
			//// 描画!(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
			//dxCommon->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);

			// 実際のcommandListのImGuiの描画コマンドを積む
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon->GetCommandList().Get());
			
			/// ----------DirectX描画処理----------
			dxCommon->PostDraw();

			///
			/// 描画処理終了
			///

			///
			/// メインループ終了
			///
		}
	}

	///
	/// 解放処理開始
	///

	/// ----------最初のシーンの解放----------

	//textureResource2.Reset();
	//textureResource.Reset();
	//vertexResource.Reset();
	//directionalLightResource.Reset();
	//materialResource.Reset();
	//wvpResource.Reset();
	//graphicsPipelineState.Reset();
	//pixelShaderBlob.Reset();
	//vertexShaderBlob.Reset();
	//rootSignature.Reset();
	//errorBlob.Reset();
	//signatureBlob.Reset();

	// 3Dオブジェクトの解放
	delete object3d;

	// スプライトの解放
	delete sprite;

	/// ----------汎用機能の解放----------

	// テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();

	// 3Dオブジェクト共通部の解放
	delete object3dCommon;

	// スプライト共通部の解放
	delete spriteCommon;

	// 入力の解放
	delete input;

	/// ----------DirectXの解放----------
	delete dxCommon;

	/// ----------ゲームウィンドウ解放----------

	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPIの解放
	delete winApp;

	D3DResourceLeakChecker leakCheck;

	///
	/// 解放処理終了
	///

	return 0;
}
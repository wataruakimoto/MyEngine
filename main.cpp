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
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	/// ----------シーンの初期化----------

	// Spriteの生成・初期化
	std::vector<Sprite*> sprites;

	for (uint32_t i = 0; i < 3; ++i) {

		Sprite* sprite = new Sprite();
		sprite->Initialize(spriteCommon);

		Vector2 position = sprite->GetPosition();
		position.x += i * 512.0f;
		sprite->SetPosition(position);

		sprites.push_back(sprite);
	}

	//// PipelineStateObject PSOの処理
	//
	//// RootSignatureを生成する
	//D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	//descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//
	//// DescriptorRangeを作成
	//D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	//descriptorRange[0].NumDescriptors = 1; // 数は1つ
	//descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	//descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
	//
	//// RootParameter作成。PixelShaderのMaterialとVertexShaderのTransform
	//D3D12_ROOT_PARAMETER rootParameters[4] = {};
	//
	//// gMaterial CBV b0
	//rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	//rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	//rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号0を使う
	//
	//// gTransformationMatrix CBV t0
	//rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	//rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
	//rootParameters[1].Descriptor.ShaderRegister = 0; // レジスタ番号0を使う
	//
	//// gTexture SRV t0
	//rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	//rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	//rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	//rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数
	//
	//// gDirectionalLight CBV b1
	//rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	//rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	//rootParameters[3].Descriptor.ShaderRegister = 1; // レジスタ番号1を使う
	//
	//descriptionRootSignature.pParameters = rootParameters; // ルートパラメータ配列のポインタ
	//descriptionRootSignature.NumParameters = _countof(rootParameters); // 配列の長さ
	//
	//// Samplerの設定
	//D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	//staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1の範囲外をリピート
	//staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	//staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	//staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	//staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	//descriptionRootSignature.pStaticSamplers = staticSamplers;
	//descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);
	//
	//// シリアライズしてバイナリにする
	//Microsoft::WRL::ComPtr <ID3DBlob> signatureBlob = nullptr;
	//Microsoft::WRL::ComPtr <ID3DBlob> errorBlob = nullptr;
	//HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature,
	//	D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	//if (FAILED(hr)) {
	//	Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	//	assert(false);
	//}
	//// バイナリを元に生成
	//Microsoft::WRL::ComPtr <ID3D12RootSignature> rootSignature = nullptr;
	//hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
	//	signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	//assert(SUCCEEDED(hr));
	//
	//// InputLayoutの設定を行う
	//D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	//
	//inputElementDescs[0].SemanticName = "POSITION";
	//inputElementDescs[0].SemanticIndex = 0;
	//inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	//
	//inputElementDescs[1].SemanticName = "TEXCOORD";
	//inputElementDescs[1].SemanticIndex = 0;
	//inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	//inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	//
	//inputElementDescs[2].SemanticName = "NORMAL";
	//inputElementDescs[2].SemanticIndex = 0;
	//inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	//
	//D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	//inputLayoutDesc.pInputElementDescs = inputElementDescs;
	//inputLayoutDesc.NumElements = _countof(inputElementDescs);
	//
	//// BlendStateの設定を行う
	//D3D12_BLEND_DESC blendDesc{};
	//// すべての色要素を書き込む
	//blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//
	//// RasterStateの設定を行う
	//D3D12_RASTERIZER_DESC rasterizerDesc{};
	//// 裏面(時計回り)を表示しない
	//rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//// 三角形の中を塗りつぶす
	//rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	//
	//// shaderをコンパイルする
	//Microsoft::WRL::ComPtr <IDxcBlob> vertexShaderBlob = dxCommon->CompileShader(L"resources/shaders/Object3D.VS.hlsl", L"vs_6_0");
	//assert(vertexShaderBlob != nullptr);
	//
	//Microsoft::WRL::ComPtr <IDxcBlob> pixelShaderBlob = dxCommon->CompileShader(L"resources/shaders/Object3D.PS.hlsl", L"ps_6_0");
	//assert(pixelShaderBlob != nullptr);
	//
	//// DepthStencilStateの設定
	//D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//// Depthの機能を有効化する
	//depthStencilDesc.DepthEnable = true;
	//// 書き込みします
	//depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//// 比較関数はLessEqual。つまり、近ければ描画される
	//depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//
	//// PSOを生成する
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	//// RootSignature
	//graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
	//// InputLayout
	//graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	//// VertexShader
	//graphicsPipelineStateDesc.VS = {
	//	vertexShaderBlob->GetBufferPointer(),
	//	vertexShaderBlob->GetBufferSize()
	//};
	//// PixelShader
	//graphicsPipelineStateDesc.PS = {
	//	pixelShaderBlob->GetBufferPointer(),
	//	pixelShaderBlob->GetBufferSize()
	//};
	//// BlendState
	//graphicsPipelineStateDesc.BlendState = blendDesc;
	//// RasterizerState
	//graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
	//// DepthStencilState
	//graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	//graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//
	//// 書き込むRTVの情報
	//graphicsPipelineStateDesc.NumRenderTargets = 1;
	//graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//// 利用するトポロジ(形状)のタイプ。三角形
	//graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//// どのように画面に色を打ち込むかの設定(気にしなくて良い)
	//graphicsPipelineStateDesc.SampleDesc.Count = 1;
	//graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//// 実際に生成
	//Microsoft::WRL::ComPtr <ID3D12PipelineState> graphicsPipelineState = nullptr;
	//hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
	//	IID_PPV_ARGS(&graphicsPipelineState));
	//assert(SUCCEEDED(hr));
	//
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
	
	// Textureを読んで転送する
	DirectX::ScratchImage mipImages = dxCommon->LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	Microsoft::WRL::ComPtr <ID3D12Resource> textureResource = dxCommon->CreateTextureResource(metadata);
	dxCommon->UploadTextureData(textureResource, mipImages);
	
	// metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = dxCommon->GetSRVCPUDescriptorHandle(1);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = dxCommon->GetSRVGPUDescriptorHandle(1);
	
	//D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = dxCommon->GetSRVCPUDescriptorHandle(1);
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = dxCommon->GetSRVGPUDescriptorHandle(1);
	
	// SRVの生成
	dxCommon->GetDevice()->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);
	
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
			
			ImGui::Begin("Sprite");
			for (Sprite* sprite : sprites) {

				if (ImGui::TreeNode("Single")) {
					// サイズ変更の確認
					Vector2 size = sprite->GetSize();
					ImGui::DragFloat2("Size", &size.x, 0.1f);
					sprite->SetSize(size);

					// 回転変更の確認
					float rotation = sprite->GetRotation();
					ImGui::DragFloat("Rotation", &rotation, 0.01f);
					sprite->SetRotation(rotation);

					// 座標変更の確認
					Vector2 position = sprite->GetPosition();
					ImGui::DragFloat2("Position", &position.x, 0.1f);
					sprite->SetPosition(position);

					// 色変更の確認
					Vector4 color = sprite->GetColor();
					ImGui::DragFloat4("Color", &color.x, 0.01f);
					sprite->SetColor(color);

					ImGui::TreePop();
				}

				sprite->Update();
			}
			ImGui::End();

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
			
			//if (ImGui::TreeNode("Camera")) {
			//
			//	ImGui::DragFloat3("Rotate", &cameraTransform.rotate.x, 0.01f);
			//	ImGui::DragFloat3("Translate", &cameraTransform.translate.x, 0.01f);
			//
			//	ImGui::TreePop();
			//}
			//
			//if (ImGui::TreeNode("Object")) {
			//
			//	ImGui::ColorEdit4("color", &materialData->color.x);
			//
			//	ImGui::SliderAngle("RotateX", &transform.rotate.x);
			//	ImGui::SliderAngle("RotateY", &transform.rotate.y);
			//	ImGui::SliderAngle("RotateZ", &transform.rotate.z);
			//	ImGui::Checkbox("enableLighting", &materialData->enableLighting);
			//	ImGui::ColorEdit4("LightColor", &directionalLightData->color.x);
			//	ImGui::DragFloat3("LightDirection", &directionalLightData->direction.x, 0.01f);
			//	ImGui::DragFloat("Intensity", &directionalLightData->intensity, 0.01f);
			//
			//	// チェックボックスによる切り替え
			//	ImGui::Checkbox("useMonsterBall", &useMonsterBall);
			//
			//	ImGui::TreePop();
			//}
			
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

			/// === Spriteの描画準備 === ///
			spriteCommon->SettingCommonDrawing();

			//TODO: 全てのSprite個々の描画
			for (Sprite* sprite : sprites) { sprite->Draw(); }

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
	textureResource.Reset();
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

	// スプライトの解放
	for (Sprite* sprite : sprites) { delete sprite; }

	/// ----------汎用機能の解放----------

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
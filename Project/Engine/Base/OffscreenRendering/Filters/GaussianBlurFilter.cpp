#include "GaussianBlurFilter.h"
#include "Base/DirectXUtility.h"
#include "Base/SrvManager.h"
#include "Base/OffscreenRendering/PostEffect.h"
#include "Debug/Logger.h"

using namespace Microsoft::WRL;
using namespace Logger;

void GaussianBlurFilter::Initialize(PostEffect* postEffect) {

	// 引数をメンバ変数にコピー
	this->postEffect = postEffect;

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// パイプライン作成
	CreateGraphicsPipeline();
}

void GaussianBlurFilter::Draw() {

	/// === ルートシグネチャをセットするコマンド === ///
	dxUtility->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	/// === グラフィックスパイプラインステートをセットするコマンド === ///
	dxUtility->GetCommandList()->SetPipelineState(graphicsPipelineState.Get());

	/// === プリミティブトポロジーをセットするコマンド === ///
	dxUtility->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D12DescriptorHeap* descriptorHeaps[] = { SrvManager::GetInstance()->GetDescriptorHeap().Get() };
	dxUtility->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);

	/// === SRVのDescriptorTableを設定 === ///
	dxUtility->GetCommandList()->SetGraphicsRootDescriptorTable(0, SrvManager::GetInstance()->GetGPUDescriptorHandle(postEffect->GetSRVIndex()));

	// 3頂点を1回描画する
	dxUtility->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void GaussianBlurFilter::CreateRootSignature() {

	// RootSignatureを生成する
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRangeを作成
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};

	// gTexture SRV t0
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成。PixelShaderのMaterialとVertexShaderのTransform
	D3D12_ROOT_PARAMETER rootParameters[1] = {};

	// gTexture SRV t0
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0]; // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数

	descriptionRootSignature.pParameters = rootParameters; // ルートパラメータ配列のポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters); // 配列の長さ

	// Samplerの作成
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};

	// gSampler s0
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う

	descriptionRootSignature.pStaticSamplers = staticSamplers; // StaticSamplerの配列のポインタ
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers); // 配列の長さ

	// シリアライズしてバイナリにする
	ComPtr <ID3DBlob> signatureBlob = nullptr;
	ComPtr <ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// バイナリを元に生成
	hr = dxUtility->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
}

void GaussianBlurFilter::CreateInputLayout() {

	// InputLayoutの設定をしない 頂点にはデータを入力しないから
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;
}

void GaussianBlurFilter::CreateBlendState() {

	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}

void GaussianBlurFilter::CreateRasterizerState() {

	// 裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
}

void GaussianBlurFilter::CreateVertexShader() {

	// シェーダコンパイルを行う
	vertexShaderBlob = dxUtility->CompileShader(L"resources/shaders/FullScreen.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);
}

void GaussianBlurFilter::CreatePixelShader() {

	// シェーダコンパイルを行う
	pixelShaderBlob = dxUtility->CompileShader(L"resources/shaders/GaussianBlur.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);
}

void GaussianBlurFilter::CreateDepthStencilState() {

	// 全画面に対して処理を行うので、比較や書き込みの必要がない
	depthStencilDesc.DepthEnable = false;
}

void GaussianBlurFilter::CreateGraphicsPipeline() {

	// PSOを生成する
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	/// === RootSignature === ///
	CreateRootSignature();
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();

	/// === InputLayout === ///
	CreateInputLayout();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;

	/// === BlendState === ///
	CreateBlendState();
	graphicsPipelineStateDesc.BlendState = blendDesc;

	/// === RasterizerState === ///
	CreateRasterizerState();
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	/// === VertexShader === ///
	CreateVertexShader();
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };

	/// === PixelShader === ///
	CreatePixelShader();
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };

	/// === DepthStencilState === ///
	CreateDepthStencilState();
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// 実際に生成
	hr = dxUtility->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}


#include "BaseFilter.h"
#include "base/DirectXUtility.h"

void BaseFilter::Initialize(DirectXUtility* dxUtility, PostEffect* postEffect) {
}

void BaseFilter::CreateInputLayout() {

	// InputLayoutの設定をしない 頂点にはデータを入力しないから
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;
}

void BaseFilter::CreateBlendState() {

	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}

void BaseFilter::CreateRasterizerState() {

	// 裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
}

void BaseFilter::CreateVertexShader() {

	// シェーダコンパイルを行う
	vertexShaderBlob = dxUtility->CompileShader(L"resources/shaders/FullScreen.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);
}

void BaseFilter::CreateDepthStencilState() {

	// 全画面に対して処理を行うので、比較や書き込みの必要がない
	depthStencilDesc.DepthEnable = false;
}

void BaseFilter::CreateGraphicsPipeline() {

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


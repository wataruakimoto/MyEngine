#include "GraphicsPipelineBase.h"
#include "GraphicsPipelineCreater.h"

GraphicsPipelineCreater::GraphicsPipelineCreater() {
}

GraphicsPipelineCreater::~GraphicsPipelineCreater() {
}

void GraphicsPipelineCreater::Initialize() {
}

void GraphicsPipelineCreater::Create() {

	// PSOを生成する
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	/// === RootSignature === ///
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
	HRESULT hr = dxUtility_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

void GraphicsPipelineCreater::CreateInputLayout() {

	// InputLayoutの設定を行う
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);
}

void GraphicsPipelineCreater::CreateBlendState() {

	// アルファブレンドで設定
	blendDesc.RenderTarget[0].BlendEnable = true; // ブレンドを有効化
	blendDesc.RenderTarget[0].LogicOpEnable = false; // 論理演算は使わない
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA; // ソースのアルファ値
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // デストの(1-ソースアルファ)
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD; // 加算
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE; // ソースのアルファ値そのまま
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO; // デストのアルファ値は使わない
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD; // 加算
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBA全て書き込み
}

void GraphicsPipelineCreater::CreateRasterizerState() {

	// 裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	// 深度クリップを有効にする
	rasterizerDesc.DepthClipEnable = true;
}

void GraphicsPipelineCreater::CreateVertexShader() {
}

void GraphicsPipelineCreater::CreatePixelShader() {
}

void GraphicsPipelineCreater::CreateDepthStencilState() {

	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みを無効にする
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

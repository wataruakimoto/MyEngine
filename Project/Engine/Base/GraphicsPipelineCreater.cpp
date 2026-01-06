#include "GraphicsPipelineCreater.h"

GraphicsPipelineCreater::GraphicsPipelineCreater() {
}

GraphicsPipelineCreater::~GraphicsPipelineCreater() {
}

void GraphicsPipelineCreater::Create(BlendMode blendMode, CullMode cullMode, DepthMode depthMode) {

	// PSOを生成する
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	/// === RootSignature === ///
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();

	/// === InputLayout === ///
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;

	/// === BlendState === ///
	CreateBlendState(blendMode);
	graphicsPipelineStateDesc.BlendState = blendDesc;

	/// === RasterizerState === ///
	CreateRasterizerState(cullMode);
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	/// === VertexShader === ///
	CreateVertexShader();
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };

	/// === PixelShader === ///
	CreatePixelShader();
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };

	/// === DepthStencilState === ///
	CreateDepthStencilState(depthMode);
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
	HRESULT hr = dxUtility_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

void GraphicsPipelineCreater::CreateBlendState(BlendMode blendMode) {

	// デフォルト値で初期化
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].LogicOpEnable = false; // 論理演算は使わない
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBA全て書き込み

	// ブレンドモードごとの設定
	switch (blendMode) {

	case BlendMode::None:

		blendDesc.RenderTarget[0].BlendEnable = false; // ブレンドを無効化

		break;

	case BlendMode::AlphaBlend:

		blendDesc.RenderTarget[0].BlendEnable = true; // ブレンドを有効化
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA; // ソースのアルファ値
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // デストの(1-ソースアルファ)
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD; // 加算
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO; // ソースのアルファ値は使わない
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE; // デストのアルファ値はそのまま
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD; // 加算

		break;
	}
}

void GraphicsPipelineCreater::CreateRasterizerState(CullMode cullMode) {

	// デフォルト値で初期化
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID; // 三角形の中を塗りつぶす
	rasterizerDesc.FrontCounterClockwise = false; // 頂点の並びが時計回りなら正面
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS; // 深度バイアス値
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP; // 深度バイアスクランプ値
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS; // 傾斜に応じた深度バイアス値
	rasterizerDesc.DepthClipEnable = true; // 深度クリッピングを有効化
	rasterizerDesc.MultisampleEnable = false; // マルチサンプルを無効化
	rasterizerDesc.AntialiasedLineEnable = false; // アンチエイリアスを無効化
	rasterizerDesc.ForcedSampleCount = 0; // 強制サンプル数
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; // 保守的ラスタライザモードを無効化

	// カリングモードごとの設定
	switch (cullMode) {

	case CullMode::Back:

		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK; // 裏面(時計回り)を表示しない

		break;

	case CullMode::Front:

		rasterizerDesc.CullMode = D3D12_CULL_MODE_FRONT; // 表面(反時計回り)を表示しない

		break;

	case CullMode::None:

		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE; // カリングしない(裏面も表示させる)

		break;
	}
}

void GraphicsPipelineCreater::CreateVertexShader() {

	// フルパスのファイル名を作成
	vertexShaderFullPath = shaderDirectoryPath + vertexShaderFileName;

	// シェーダーをコンパイル
	vertexShaderBlob = dxUtility_->CompileShader(vertexShaderFullPath, L"vs_6_0");
	assert(vertexShaderBlob != nullptr);
}

void GraphicsPipelineCreater::CreatePixelShader() {

	// フルパスのファイル名を作成
	pixelShaderFullPath = shaderDirectoryPath + pixelShaderFileName;

	// シェーダーをコンパイル
	pixelShaderBlob = dxUtility_->CompileShader(pixelShaderFullPath, L"ps_6_0");
	assert(pixelShaderBlob != nullptr);
}

void GraphicsPipelineCreater::CreateDepthStencilState(DepthMode depthMode) {

	// デフォルト値で初期化
	depthStencilDesc.DepthEnable = true; // Depthの機能を有効化
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 書き込み可能
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さい値ほど手前に描画

	// デプスモードごとの設定
	switch (depthMode) {

	case DepthMode::ReadWrite:

		// デフォルト設定のまま

		break;

	case DepthMode::ReadOnly:

		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; // 読み込み専用

		break;

	case DepthMode::Disabled:

		depthStencilDesc.DepthEnable = false; // Depthの機能を無効化

		break;
	}
}

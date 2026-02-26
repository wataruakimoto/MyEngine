#include "GraphicsPipelineBuilder.h"
#include "DirectXUtility.h"

#include <cassert>

using namespace Engine;

GraphicsPipelineBuilder::GraphicsPipelineBuilder() {
}

GraphicsPipelineBuilder::~GraphicsPipelineBuilder() {
}

void GraphicsPipelineBuilder::AddInputElement(const char* semanticName, UINT semanticIndex, DXGI_FORMAT format) {

	D3D12_INPUT_ELEMENT_DESC inputElementDesc{};

	inputElementDesc.SemanticName = semanticName;
	inputElementDesc.SemanticIndex = semanticIndex;
	inputElementDesc.Format = format;
	inputElementDesc.InputSlot = 0;
	inputElementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

	inputElementDescs_.push_back(inputElementDesc);
}

void GraphicsPipelineBuilder::Build() {

	// PipelineStateの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	
	// RootSignatureの設定
	ConfigureRootSignature();
	graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();

	// VertexShaderの設定
	ConfigureVertexShader();
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() };

	// PixelShaderの設定
	ConfigurePixelShader();
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() };

	// BlendStateの設定
	ConfigureBlendState();
	graphicsPipelineStateDesc.BlendState = blendDesc_;

	// SamplerStateの設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// RasterizerStateの設定
	ConfigureRasterizerState();
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;

	// DepthStencilStateの設定
	ConfigureDepthStencilState();
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc_;

	// InputLayoutの設定
	ConfigureInputLayout();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;

	// PrimitiveTopologyTypeの設定
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// RTVFormatsの設定
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// GraphicsPipelineStateの生成
}

void GraphicsPipelineBuilder::ConfigureRootSignature() {
}

void GraphicsPipelineBuilder::ConfigureVertexShader() {

	// シェーダーのフルパスを作成
	std::wstring vertexShaderFullPath = shaderDirectoryPath_ + vertexShaderFileName_;

	// シェーダーをコンパイル
	vertexShaderBlob_ = DirectXUtility::GetInstance()->CompileShader(vertexShaderFullPath, L"vs_5_0");
	assert(vertexShaderBlob_ != nullptr);
}

void GraphicsPipelineBuilder::ConfigurePixelShader() {

	// シェーダーのフルパスを作成
	std::wstring pixelShaderFullPath = shaderDirectoryPath_ + pixelShaderFileName_;

	// シェーダーをコンパイル
	pixelShaderBlob_ = DirectXUtility::GetInstance()->CompileShader(pixelShaderFullPath, L"ps_5_0");
	assert(pixelShaderBlob_ != nullptr);
}

void GraphicsPipelineBuilder::ConfigureBlendState() {

	/// ===== BlendStateの基本設定 ===== ///
	
	blendDesc_.AlphaToCoverageEnable = false;
	blendDesc_.IndependentBlendEnable = false;

	/// ===== RenderTarget[0]への設定 ===== ///

	D3D12_RENDER_TARGET_BLEND_DESC& renderTargetBlendDesc = blendDesc_.RenderTarget[0];

	// モードごとの設定
	switch (blendMode_) {

	// なし
	case BlendMode::None:

		renderTargetBlendDesc.BlendEnable = false; // ブレンドを無効化

		break;

	// アルファ
	case BlendMode::Alpha:

		renderTargetBlendDesc.BlendEnable = true;					 // ブレンドを有効化
		renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		 // ソースのアルファ値
		renderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // デストの(1-ソースアルファ)
		renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;			 // 加算
		renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		 // ソースのアルファ値はそのまま
		renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	 // デストのアルファ値は使わない
		renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	 // 加算

		break;

	// 加算
	case BlendMode::Add:

		renderTargetBlendDesc.BlendEnable = true;				 // ブレンドを有効化
		renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;	 // ソースのアルファ値
		renderTargetBlendDesc.DestBlend = D3D12_BLEND_ONE;		 // デストの値をそのまま
		renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;		 // 加算
		renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;	 // ソースのアルファ値はそのまま
		renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO; // デストのアルファ値は使わない
		renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; // 加算

		break;

	// 減算
	case BlendMode::Subtract:

		renderTargetBlendDesc.BlendEnable = true;					 // ブレンドを有効化
		renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		 // ソースのアルファ値
		renderTargetBlendDesc.DestBlend = D3D12_BLEND_ONE;			 // デストの値をそのまま
		renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT; // 減算
		renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		 // ソースのアルファ値はそのまま
		renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	 // デストのアルファ値は使わない
		renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	 // 加算

		break;

	// 乗算
	case BlendMode::Multiply:

		renderTargetBlendDesc.BlendEnable = true;				 // ブレンドの有効化
		renderTargetBlendDesc.SrcBlend = D3D12_BLEND_ZERO;		 // ソースの値は使わない
		renderTargetBlendDesc.DestBlend = D3D12_BLEND_SRC_COLOR; // デストの値にソースの値を乗算
		renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;		 // 加算
		renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;	 // ソースのアルファ値はそのまま
		renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO; // デストのアルファ値は使わない
		renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; // 加算

		break;
	}

	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBA全て書き込む
}

void GraphicsPipelineBuilder::ConfigureRasterizerState() {

	/// ===== ラスタライザーステートの基本設定 ===== ///

	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID; // 三角形の中を塗りつぶす

	// モードごとの設定
	switch (cullMode_) {
	
	// カリングなし
	case CullMode::None:

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE; // カリングしない(裏面も表示させる)

		break;

	// 前面カリング
	case CullMode::Front:

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_FRONT; // 表面(反時計回り)を表示しない

		break;

	// 背面カリング
	case CullMode::Back:

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK; // 裏面(時計回り)を表示しない

		break;
	}

	rasterizerDesc_.FrontCounterClockwise = false;								    // 頂点の並びが時計回りなら正面
	rasterizerDesc_.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;						    // 深度バイアス値
	rasterizerDesc_.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;			    // 深度バイアスクランプ値
	rasterizerDesc_.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;   // 傾斜に応じた深度バイアス値
	rasterizerDesc_.DepthClipEnable = true;										    // 深度クリッピングを有効化
	rasterizerDesc_.MultisampleEnable = false;									    // マルチサンプルを無効化
	rasterizerDesc_.AntialiasedLineEnable = false;								    // アンチエイリアスを無効化
	rasterizerDesc_.ForcedSampleCount = 0;										    // 強制サンプル数
	rasterizerDesc_.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; // 保守的ラスタライザモードを無効化
}

void GraphicsPipelineBuilder::ConfigureDepthStencilState() {

	/// ===== 深度ステンシルステートの基本設定 ===== ///

	// モードごとの設定
	switch (depthMode_) {

	// 無効化
	case DepthMode::Disabled:

		depthStencilDesc_.DepthEnable = false;						    // 深度テストを無効化
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; // 深度値の書き込みを禁止

		break;

	// 読み込みのみ
	case DepthMode::ReadOnly:

		depthStencilDesc_.DepthEnable = true;							// 深度テストを有効化
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; // 深度値の書き込みを禁止

		break;

	// 読み書き可能
	case DepthMode::ReadWrite:

		depthStencilDesc_.DepthEnable = true;						   // 深度テストを有効化
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 深度値の書き込みを許可

		break;
	}

	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さい値ほど手前に描画
	depthStencilDesc_.StencilEnable = false;				  // ステンシルテストを無効化
}

void GraphicsPipelineBuilder::ConfigureInputLayout() {
}

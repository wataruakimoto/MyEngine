#include "GraphicsPipelineBuilder.h"
#include "DirectXUtility.h"
#include "Logger.h"

#include <cassert>

using namespace Engine;
using namespace Logger;
using namespace Microsoft::WRL;

GraphicsPipelineBuilder::GraphicsPipelineBuilder() {
}

GraphicsPipelineBuilder::~GraphicsPipelineBuilder() {
}

void GraphicsPipelineBuilder::AddRootParameter(D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility) {

	// CBV、SRV、UAVのいずれかでなければエラー
	assert(type == D3D12_ROOT_PARAMETER_TYPE_CBV || type == D3D12_ROOT_PARAMETER_TYPE_SRV || type == D3D12_ROOT_PARAMETER_TYPE_UAV);

	// ルートパラメーターの設定
	D3D12_ROOT_PARAMETER rootParameter{};

	rootParameter.ParameterType = type;
	rootParameter.Descriptor.ShaderRegister = shaderRegister;
	rootParameter.Descriptor.RegisterSpace = 0;
	rootParameter.ShaderVisibility = visibility;

	// ルートパラメータの配列に追加
	rootParameters_.push_back(rootParameter);
}

void GraphicsPipelineBuilder::AddRootParameterTable(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT baseShaderRegister, D3D12_SHADER_VISIBILITY visibility) {

	// ディスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	descriptorRange.RangeType = type;
	descriptorRange.NumDescriptors = 1;
	descriptorRange.BaseShaderRegister = baseShaderRegister;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ディスクリプタレンジの配列に追加
	descriptorRanges_.push_back(descriptorRange);

	// ルートパラメーターの設定
	D3D12_ROOT_PARAMETER rootParameter{};

	rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter.DescriptorTable.pDescriptorRanges = &descriptorRanges_.back();
	rootParameter.DescriptorTable.NumDescriptorRanges = 1;
	rootParameter.ShaderVisibility = visibility;

	// ルートパラメータの配列に追加
	rootParameters_.push_back(rootParameter);
}

void GraphicsPipelineBuilder::AddStaticSampler(D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE addressMode, UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility) {

	// 静的サンプラーの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplerDesc{};

	staticSamplerDesc.Filter = filter;
	staticSamplerDesc.AddressU = addressMode;
	staticSamplerDesc.AddressV = addressMode;
	staticSamplerDesc.AddressW = addressMode;
	staticSamplerDesc.MipLODBias = 0.0f;
	staticSamplerDesc.MaxAnisotropy = 1;
	staticSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	staticSamplerDesc.MinLOD = 0.0f;
	staticSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplerDesc.ShaderRegister = shaderRegister;
	staticSamplerDesc.RegisterSpace = 0;
	staticSamplerDesc.ShaderVisibility = visibility;

	// 静的サンプラーの設定
	staticSamplerDescs_.push_back(staticSamplerDesc);
}

void GraphicsPipelineBuilder::AddInputElement(const char* semanticName, UINT semanticIndex, DXGI_FORMAT format) {

	// インプットエレメントの設定
	D3D12_INPUT_ELEMENT_DESC inputElementDesc{};

	inputElementDesc.SemanticName = semanticName;
	inputElementDesc.SemanticIndex = semanticIndex;
	inputElementDesc.Format = format;
	inputElementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	// インプットエレメントの配列に追加
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
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleDesc.Quality = 0;

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
	ConfigureTopologyType();
	graphicsPipelineStateDesc.PrimitiveTopologyType = primitiveTopologyType_;

	// RTVFormatsの設定
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// GraphicsPipelineStateの生成
	HRESULT hr = dxUtility_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}

void GraphicsPipelineBuilder::Reset() {

	/// ===== RootSignature ===== ///

	rootSignature_.Reset();
	descriptorRanges_.clear();
	rootParameters_.clear();
	staticSamplerDescs_.clear();

	/// ===== Shader ===== ///

	vertexShaderFileName_.clear();
	vertexShaderBlob_.Reset();
	pixelShaderFileName_.clear();
	pixelShaderBlob_.Reset();

	/// ===== BlendState ===== ///

	blendDesc_ = {};
	blendMode_ = BlendMode::None;

	/// ===== RasterizerState ===== ///
	
	rasterizerDesc_ = {};
	cullMode_ = CullMode::None;

	/// ===== DepthStencilState ===== ///

	depthStencilDesc_ = {};
	depthMode_ = DepthMode::Disabled;

	/// ===== InputLayout ===== ///

	inputElementDescs_.clear();
	inputLayoutDesc_ = {};

	/// ===== TopologyType ===== ///

	topologyMode_ = TopologyMode::Triangle;
	primitiveTopologyType_ = {};

	/// ===== Pipeline ===== ///

	graphicsPipelineState_.Reset();
}

void GraphicsPipelineBuilder::ConfigureRootSignature() {

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};

	// IAステージでInputLayoutを使う
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootParameters_.data(); // ルートパラメーター配列のポインタ
	rootSignatureDesc.NumParameters = static_cast<UINT>(rootParameters_.size()); // ルートパラメーターの数

	if (staticSamplerDescs_.size() > 0) {

		rootSignatureDesc.pStaticSamplers = staticSamplerDescs_.data(); // 静的サンプラーの配列のポインタ
		rootSignatureDesc.NumStaticSamplers = static_cast<UINT>(staticSamplerDescs_.size()); // 静的サンプラーの数
	}

	// ルートシグネチャを生成する

	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	// シリアライズしてバイナリにする
	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// バイナリを元にルートシグネチャを生成
	hr = DirectXUtility::GetInstance()->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));

	// 成功したか確認
	assert(SUCCEEDED(hr));
}

void GraphicsPipelineBuilder::ConfigureVertexShader() {

	// シェーダーのフルパスを作成
	std::wstring vertexShaderFullPath = shaderDirectoryPath_ + vertexShaderFileName_;

	// シェーダーをコンパイル
	vertexShaderBlob_ = DirectXUtility::GetInstance()->CompileShader(vertexShaderFullPath, L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);
}

void GraphicsPipelineBuilder::ConfigurePixelShader() {

	// シェーダーのフルパスを作成
	std::wstring pixelShaderFullPath = shaderDirectoryPath_ + pixelShaderFileName_;

	// シェーダーをコンパイル
	pixelShaderBlob_ = DirectXUtility::GetInstance()->CompileShader(pixelShaderFullPath, L"ps_6_0");
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
		renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ZERO;		 // ソースのアルファ値はそのまま
		renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ONE;	 // デストのアルファ値は使わない
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

	// インプットレイアウトの設定
	inputLayoutDesc_.pInputElementDescs = inputElementDescs_.data(); // インプットエレメントの配列のポインタ
	inputLayoutDesc_.NumElements = static_cast<UINT>(inputElementDescs_.size()); // インプットエレメントの数
}

void GraphicsPipelineBuilder::ConfigureTopologyType() {

	switch (topologyMode_) {

	case GraphicsPipelineBuilder::TopologyMode::Point:

		primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

		break;

	case GraphicsPipelineBuilder::TopologyMode::Line:

		primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

		break;

	case GraphicsPipelineBuilder::TopologyMode::Triangle:

		primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		break;
	}
}

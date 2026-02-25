#include "GraphicsPipelineBuilder.h"

using namespace Engine;

GraphicsPipelineBuilder::GraphicsPipelineBuilder() {
}

GraphicsPipelineBuilder::~GraphicsPipelineBuilder() {
}

void GraphicsPipelineBuilder::ConfigureBlendState() {

	/// ===== BlendStateの基本設定 ===== ///
	
	blendDesc_.AlphaToCoverageEnable = false;
	blendDesc_.IndependentBlendEnable = false;

	/// ===== RenderTarget[0]への設定 ===== ///

	D3D12_RENDER_TARGET_BLEND_DESC& renderTargetBlendDesc = blendDesc_.RenderTarget[0];

	// ブレンドモードに応じて設定
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
		renderTargetBlendDesc.DestBlend = D3D12_BLEND_SRC_COLOR; //
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

	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID; //

	// カリングモードに応じて設定
	switch (cullMode_) {
	
	// カリングなし
	case CullMode::None:

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;

		break;

	// 前面カリング
	case CullMode::Front:

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_FRONT;

		break;

	// 背面カリング
	case CullMode::Back:

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;

		break;
	}
}

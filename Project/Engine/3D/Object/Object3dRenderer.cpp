#include "Object3dRenderer.h"
#include "DirectXUtility.h"
#include "SrvManager.h"

using namespace Engine;

void Object3dRenderer::Initialize() {

	// DirectXユーティリティのインスタンス取得
	dxUtility_ = DirectXUtility::GetInstance();

	// SrvManagerのインスタンス取得
	srvManager_ = SrvManager::GetInstance();

	// 不透明用のグラフィックスパイプラインの生成
	CreateGraphicsPipelinOpaque();

	// 透明用のグラフィックスパイプラインの生成
	CreateGraphicsPipelinAlpha();
}

void Object3dRenderer::SettingDrawingOpaque() {

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = dxUtility_->GetCommandList().Get();

	// ルートシグネチャを設定
	commandList->SetGraphicsRootSignature(pipelineBuilderOpaque_.GetRootSignature().Get());

	// パイプラインを設定
	commandList->SetPipelineState(pipelineBuilderOpaque_.GetGraphicsPipeline().Get());

	// プリミティブトポロジーを線で設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ディスクリプタヒープを取得
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvManager_->GetDescriptorHeap().Get() };

	// ディスクリプタヒープを設定
	dxUtility_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void Object3dRenderer::SettingDrawingAlpha() {

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = dxUtility_->GetCommandList().Get();

	// ルートシグネチャを設定
	commandList->SetGraphicsRootSignature(pipelineBuilderAlpha_.GetRootSignature().Get());

	// パイプラインを設定
	commandList->SetPipelineState(pipelineBuilderAlpha_.GetGraphicsPipeline().Get());

	// プリミティブトポロジーを線で設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ディスクリプタヒープを取得
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvManager_->GetDescriptorHeap().Get() };

	// ディスクリプタヒープを設定
	dxUtility_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void Object3dRenderer::Finalize() {

	delete instance_;
	instance_ = nullptr;
}

Object3dRenderer* Object3dRenderer::instance_ = nullptr;

void Object3dRenderer::CreateGraphicsPipelinOpaque() {

	// gTransformationMatrix CBV、b0、頂点シェーダーで使う
	pipelineBuilderOpaque_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	// gMaterial CBV、b1、ピクセルシェーダーで使う
	pipelineBuilderOpaque_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, D3D12_SHADER_VISIBILITY_PIXEL);

	// gDirectionalLight CBV、b2、ピクセルシェーダーで使う
	pipelineBuilderOpaque_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 2, D3D12_SHADER_VISIBILITY_PIXEL);

	// gPointLight CBV、b3、ピクセルシェーダーで使う
	pipelineBuilderOpaque_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 3, D3D12_SHADER_VISIBILITY_PIXEL);

	// gSpotLight CBV、b4、ピクセルシェーダーで使う
	pipelineBuilderOpaque_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 4, D3D12_SHADER_VISIBILITY_PIXEL);

	// gEnvironment CBV、b5、ピクセルシェーダーで使う
	pipelineBuilderOpaque_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 5, D3D12_SHADER_VISIBILITY_PIXEL);

	// gCamera CBV、b6、ピクセルシェーダーで使う
	pipelineBuilderOpaque_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 6, D3D12_SHADER_VISIBILITY_PIXEL);

	// gTexture SRV、t0、ピクセルシェーダーで使う
	pipelineBuilderOpaque_.AddRootParameterTable(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// gEnvironmentTexture SRV、t1、ピクセルシェーダーで使う
	pipelineBuilderOpaque_.AddRootParameterTable(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, D3D12_SHADER_VISIBILITY_PIXEL);

	// gSampler 線形フィルタ、テクスチャ端は繰り返し、s0、ピクセルシェーダーで使う
	pipelineBuilderOpaque_.AddStaticSampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// シェーダーをパイプラインに設定
	pipelineBuilderOpaque_.SetVertexShaderFileName(vertexShaderFileName);
	pipelineBuilderOpaque_.SetPixelShaderFileName(pixelShaderFileName);

	// ブレンドモードの設定 アルファブレンド
	pipelineBuilderOpaque_.SetBlendMode(GraphicsPipelineBuilder::BlendMode::None);

	// カリングモードの設定 なし
	pipelineBuilderOpaque_.SetCullMode(GraphicsPipelineBuilder::CullMode::Back);

	// 深度モードの設定 無効化
	pipelineBuilderOpaque_.SetDepthMode(GraphicsPipelineBuilder::DepthMode::ReadWrite);

	// インプットエレメントの追加 POSITION0 float4
	pipelineBuilderOpaque_.AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// インプットエレメントの追加 TEXCOORD0 float2
	pipelineBuilderOpaque_.AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);

	// インプットエレメントの追加 NORMAL0 float3
	pipelineBuilderOpaque_.AddInputElement("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	// トポロジーモードの設定
	pipelineBuilderOpaque_.SetTopologyMode(GraphicsPipelineBuilder::TopologyMode::Triangle);

	// パイプライン作成
	pipelineBuilderOpaque_.Build();
}

void Object3dRenderer::CreateGraphicsPipelinAlpha() {

	// gTransformationMatrix CBV、b0、頂点シェーダーで使う
	pipelineBuilderAlpha_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	// gMaterial CBV、b1、ピクセルシェーダーで使う
	pipelineBuilderAlpha_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, D3D12_SHADER_VISIBILITY_PIXEL);

	// gDirectionalLight CBV、b2、ピクセルシェーダーで使う
	pipelineBuilderAlpha_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 2, D3D12_SHADER_VISIBILITY_PIXEL);

	// gPointLight CBV、b3、ピクセルシェーダーで使う
	pipelineBuilderAlpha_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 3, D3D12_SHADER_VISIBILITY_PIXEL);

	// gSpotLight CBV、b4、ピクセルシェーダーで使う
	pipelineBuilderAlpha_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 4, D3D12_SHADER_VISIBILITY_PIXEL);

	// gEnvironment CBV、b5、ピクセルシェーダーで使う
	pipelineBuilderAlpha_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 5, D3D12_SHADER_VISIBILITY_PIXEL);

	// gCamera CBV、b6、ピクセルシェーダーで使う
	pipelineBuilderAlpha_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 6, D3D12_SHADER_VISIBILITY_PIXEL);

	// gTexture SRV、t0、ピクセルシェーダーで使う
	pipelineBuilderAlpha_.AddRootParameterTable(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// gEnvironmentTexture SRV、t1、ピクセルシェーダーで使う
	pipelineBuilderAlpha_.AddRootParameterTable(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, D3D12_SHADER_VISIBILITY_PIXEL);

	// gSampler 線形フィルタ、テクスチャ端は繰り返し、s0、ピクセルシェーダーで使う
	pipelineBuilderAlpha_.AddStaticSampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// シェーダーをパイプラインに設定
	pipelineBuilderAlpha_.SetVertexShaderFileName(vertexShaderFileName);
	pipelineBuilderAlpha_.SetPixelShaderFileName(pixelShaderFileName);

	// ブレンドモードの設定 アルファブレンド
	pipelineBuilderAlpha_.SetBlendMode(GraphicsPipelineBuilder::BlendMode::Alpha);

	// カリングモードの設定 なし
	pipelineBuilderAlpha_.SetCullMode(GraphicsPipelineBuilder::CullMode::Back);

	// 深度モードの設定 無効化
	pipelineBuilderAlpha_.SetDepthMode(GraphicsPipelineBuilder::DepthMode::ReadOnly);

	// インプットエレメントの追加 POSITION0 float4
	pipelineBuilderAlpha_.AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// インプットエレメントの追加 TEXCOORD0 float2
	pipelineBuilderAlpha_.AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);

	// インプットエレメントの追加 NORMAL0 float3
	pipelineBuilderAlpha_.AddInputElement("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	// トポロジーモードの設定
	pipelineBuilderAlpha_.SetTopologyMode(GraphicsPipelineBuilder::TopologyMode::Triangle);

	// パイプライン作成
	pipelineBuilderAlpha_.Build();
}

Object3dRenderer* Object3dRenderer::GetInstance() {

	if (instance_ == nullptr) {
		instance_ = new Object3dRenderer;
	}
	return instance_;
}

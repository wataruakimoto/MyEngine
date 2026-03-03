#include "SkyBoxRenderer.h"
#include "DirectXUtility.h"
#include "SrvManager.h"

using namespace Engine;

void SkyBoxRenderer::Initialize() {

	// DirectXユーティリティのインスタンス取得
	dxUtility_ = DirectXUtility::GetInstance();

	// SrvManagerのインスタンス取得
	srvManager_ = SrvManager::GetInstance();

	// gTransformationMatrix CBV b0 頂点シェーダーで使う
	pipelineBuilder_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	// gMaterial CBV b1 ピクセルシェーダーで使う
	pipelineBuilder_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, D3D12_SHADER_VISIBILITY_PIXEL);

	// gTexture SRV t0 ピクセルシェーダーで使う
	pipelineBuilder_.AddRootParameterTable(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// gSampler 線形フィルタ、テクスチャ端は繰り返し、s0、ピクセルシェーダーで使う
	pipelineBuilder_.AddStaticSampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// シェーダーをパイプラインに設定
	pipelineBuilder_.SetVertexShaderFileName(vertexShaderFileName);
	pipelineBuilder_.SetPixelShaderFileName(pixelShaderFileName);

	// ブレンドモードの設定 アルファブレンド
	pipelineBuilder_.SetBlendMode(GraphicsPipelineBuilder::BlendMode::None);

	// カリングモードの設定 なし
	pipelineBuilder_.SetCullMode(GraphicsPipelineBuilder::CullMode::None);

	// 深度モードの設定 無効化
	pipelineBuilder_.SetDepthMode(GraphicsPipelineBuilder::DepthMode::ReadOnly);

	// インプットエレメントの追加 POSITION0 float4
	pipelineBuilder_.AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// インプットエレメントの追加 TEXCOORD0 float2
	pipelineBuilder_.AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);

	// トポロジーモードの設定
	pipelineBuilder_.SetTopologyMode(GraphicsPipelineBuilder::TopologyMode::Triangle);

	// パイプライン作成
	pipelineBuilder_.Build();
}

void SkyBoxRenderer::SettingDrawing() {

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = dxUtility_->GetCommandList().Get();

	// ルートシグネチャを設定
	commandList->SetGraphicsRootSignature(pipelineBuilder_.GetRootSignature().Get());

	// パイプラインを設定
	commandList->SetPipelineState(pipelineBuilder_.GetGraphicsPipeline().Get());

	// プリミティブトポロジーを線で設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ディスクリプタヒープを取得
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvManager_->GetDescriptorHeap().Get() };

	// ディスクリプタヒープを設定
	dxUtility_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void SkyBoxRenderer::Finalize() {

	delete instance_;
	instance_ = nullptr;
}

SkyBoxRenderer* SkyBoxRenderer::instance_ = nullptr;

SkyBoxRenderer* SkyBoxRenderer::GetInstance() {

	if (instance_ == nullptr) {
		instance_ = new SkyBoxRenderer;
	}
	return instance_;
}

#include "LineRenderer.h"
#include "DirectXUtility.h"
#include "Logger.h"

using namespace Engine;
using namespace Microsoft::WRL;

void LineRenderer::Initialize() {

	// DirectXユーティリティのインスタンス取得
	dxUtility_ = DirectXUtility::GetInstance();

	// gTransformationMatrix CBV b0 頂点シェーダーで使う
	pipelineBuilder_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	// シェーダーをパイプラインに設定
	pipelineBuilder_.SetVertexShaderFileName(vertexShaderFileName);
	pipelineBuilder_.SetPixelShaderFileName(pixelShaderFileName);

	// ブレンドモードの設定 アルファブレンド
	pipelineBuilder_.SetBlendMode(GraphicsPipelineBuilder::BlendMode::Alpha);

	// カリングモードの設定 なし
	pipelineBuilder_.SetCullMode(GraphicsPipelineBuilder::CullMode::None);

	// 深度モードの設定 無効化
	pipelineBuilder_.SetDepthMode(GraphicsPipelineBuilder::DepthMode::Disabled);

	// インプットエレメントの追加 POSITION0 float4
	pipelineBuilder_.AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// インプットエレメントの追加 COLOR0 float4
	pipelineBuilder_.AddInputElement("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// トポロジーモードの設定
	pipelineBuilder_.SetTopologyMode(GraphicsPipelineBuilder::TopologyMode::Line);

	// パイプライン作成
	pipelineBuilder_.Build();
}

void LineRenderer::SettingDrawing() {

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = dxUtility_->GetCommandList().Get();

	// ルートシグネチャを設定
	commandList->SetGraphicsRootSignature(pipelineBuilder_.GetRootSignature().Get());

	// パイプラインを設定
	commandList->SetPipelineState(pipelineBuilder_.GetGraphicsPipeline().Get());

	// プリミティブトポロジーを線で設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void LineRenderer::Finalize() {

	delete instance_;
	instance_ = nullptr;
}

LineRenderer* LineRenderer::instance_ = nullptr;

LineRenderer* LineRenderer::GetInstance() {
	
	if (instance_ == nullptr) {
		instance_ = new LineRenderer;
	}
	return instance_;
}

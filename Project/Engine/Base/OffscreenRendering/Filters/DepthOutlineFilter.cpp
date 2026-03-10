#include "DepthOutlineFilter.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "Camera.h"
#include "MathMatrix.h"

#include <imgui.h>

using namespace Engine;
using namespace Microsoft::WRL;
using namespace MathMatrix;

void DepthOutlineFilter::Initialize() {

	// DirectXユーティリティのインスタンス取得
	dxUtility_ = DirectXUtility::GetInstance();

	// SrvManagerのインスタンス取得
	srvManager_ = SrvManager::GetInstance();

	// グラフィックスパイプラインの生成
	CreateGraphicsPipeline();

	// コンフィグデータの生成
	CreateConfigData();
}

void DepthOutlineFilter::Draw() {

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

	// ConfigのCBVをルートパラメータに設定
	dxUtility_->GetCommandList()->SetGraphicsRootConstantBufferView(0, configBuffer_->GetGPUVirtualAddress());

	// SRVをルートパラメータに設定
	dxUtility_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(srvIndex_));

	// 深度SRVをルートパラメータに設定
	dxUtility_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(depthSrvIndex));

	// 3頂点を1回描画する
	dxUtility_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void DepthOutlineFilter::ShowImGui() {

#ifdef USE_IMGUI

	if (ImGui::TreeNode("DepthOutlineFilter")) {

		// 有効化フラグのチェックボックス
		ImGui::Checkbox("IsActive", &isActive_);
		// ImGuiのツリーを閉じる
		ImGui::TreePop();
	}

#endif // USE_IMGUI
}

void DepthOutlineFilter::CreateGraphicsPipeline() {

	// gConfig CBV b0 ピクセルシェーダーで使う
	pipelineBuilder_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// gTexture SRV t0 ピクセルシェーダーで使う
	pipelineBuilder_.AddRootParameterTable(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// gDepthTexture SRV t1 ピクセルシェーダーで使う
	pipelineBuilder_.AddRootParameterTable(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, D3D12_SHADER_VISIBILITY_PIXEL);

	// gSampler 線形フィルタ、テクスチャ端は繰り返し、s0、ピクセルシェーダーで使う
	pipelineBuilder_.AddStaticSampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// gSamplerPoint ポイントフィルタ、テクスチャ端は繰り返し、s1、ピクセルシェーダーで使う
	pipelineBuilder_.AddStaticSampler(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 1, D3D12_SHADER_VISIBILITY_PIXEL);

	// シェーダーをパイプラインに設定
	pipelineBuilder_.SetVertexShaderFileName(vertexShaderFileName_);
	pipelineBuilder_.SetPixelShaderFileName(pixelShaderFileName_);

	// ブレンドモードの設定 アルファブレンド
	pipelineBuilder_.SetBlendMode(GraphicsPipelineBuilder::BlendMode::None);

	// カリングモードの設定 なし
	pipelineBuilder_.SetCullMode(GraphicsPipelineBuilder::CullMode::None);

	// 深度モードの設定 無効化
	pipelineBuilder_.SetDepthMode(GraphicsPipelineBuilder::DepthMode::Disabled);

	// インプットエレメントの追加 POSITION0 float4
	pipelineBuilder_.AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// インプットエレメントの追加 TEXCOORD0 float2
	pipelineBuilder_.AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);

	// トポロジーモードの設定
	pipelineBuilder_.SetTopologyMode(GraphicsPipelineBuilder::TopologyMode::Triangle);

	// パイプライン作成
	pipelineBuilder_.Build();
}

void DepthOutlineFilter::CreateConfigData() {

	// コンフィグバッファを生成
	configBuffer_ = dxUtility_->CreateBufferResource(sizeof(ConfigData));

	// VRAMにデータを書き込むためにアドレスを取得して割り当てる
	configBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&configData_));

	// 初期値をセット
	configData_->projectionInverse = MakeIdentity4x4(); // 単位行列で初期化
}


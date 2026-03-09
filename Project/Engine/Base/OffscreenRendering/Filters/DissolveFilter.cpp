#include "DissolveFilter.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "Texture/TextureManager.h"

#include <imgui.h>

using namespace Engine;
using namespace Microsoft::WRL;

void DissolveFilter::Initialize() {

	// DirectXユーティリティのインスタンス取得
	dxUtility_ = DirectXUtility::GetInstance();

	// SrvManagerのインスタンス取得
	srvManager_ = SrvManager::GetInstance();

	// グラフィックスパイプラインの生成
	CreateGraphicsPipeline();

	// コンフィグデータの生成
	CreateConfigData();
}

void DissolveFilter::Draw() {

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

	// マスクテクスチャのSRVをルートパラメータに設定
	dxUtility_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(maskTextureSrvIndex));

	// 3頂点を1回描画する
	dxUtility_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void DissolveFilter::ShowImGui() {

#ifdef USE_IMGUI

	if (ImGui::TreeNode("DissolveFilter")) {

		// 有効化フラグのチェックボックス
		ImGui::Checkbox("IsActive", &isActive_);

		ImGui::SliderFloat("Threshold", &configData_->threshold, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat("DetectionRange", &configData_->detectionRange, 0.0f, 0.1f, "%.3f");
		ImGui::ColorEdit3("EdgeColor", &configData_->edgeColor.x);

		// ImGuiのツリーを閉じる
		ImGui::TreePop();
	}
	
#endif // USE_IMGUI
}

void DissolveFilter::CreateGraphicsPipeline() {

	// gConfig CBV b0 ピクセルシェーダーで使う
	pipelineBuilder_.AddRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// gTexture SRV t0 ピクセルシェーダーで使う
	pipelineBuilder_.AddRootParameterTable(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	// gMaskTexture SRV t1 ピクセルシェーダーで使う
	pipelineBuilder_.AddRootParameterTable(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, D3D12_SHADER_VISIBILITY_PIXEL);

	// gSampler 線形フィルタ、テクスチャ端は繰り返し、s0、ピクセルシェーダーで使う
	pipelineBuilder_.AddStaticSampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 0, D3D12_SHADER_VISIBILITY_PIXEL);

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

void DissolveFilter::CreateConfigData() {

	// コンフィグバッファを生成
	configBuffer_ = dxUtility_->CreateBufferResource(sizeof(ConfigData));

	// VRAMにデータを書き込むためにアドレスを取得して割り当てる
	configBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&configData_));

	// データの初期化
	configData_->threshold = 0.0f; // 閾値を0.0に設定
	configData_->detectionRange = 0.0f; // 検出範囲を0.0に設定
	configData_->edgeColor = { 0.0f,0.0f,0.0f }; // 黒に設定
}

void DissolveFilter::SetMaskTextureFilePath(const std::string relativePath) {

	maskTextureFullPath = TextureManager::GetInstance()->GetBaseDirectoryPath() + "/" + relativePath;

	// テクスチャを読み込む
	TextureManager::GetInstance()->LoadTexture(maskTextureFullPath);

	// SRVインデックスを取得
	maskTextureSrvIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(maskTextureFullPath);
}


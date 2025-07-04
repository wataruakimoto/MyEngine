#include "SkyboxCommon.h"
#include "base/DirectXUtility.h"
#include "base/SrvManager.h"
#include "debug/Logger.h"

using namespace Microsoft::WRL;

void SkyboxCommon::Initialize(DirectXUtility* dxUtility) {

	// 引数をメンバ変数に代入
	dxUtility_ = dxUtility;

	// グラフィックスパイプラインの生成
	CreateGraphicsPipeline();
}

void SkyboxCommon::SettingDrawing() {

	/// === ルートシグネチャをセットするコマンド === ///
	dxUtility_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	/// === グラフィックスパイプラインステートをセットするコマンド === ///
	dxUtility_->GetCommandList()->SetPipelineState(graphicsPipelineState.Get());

	/// === プリミティブトポロジーをセットするコマンド === ///
	dxUtility_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D12DescriptorHeap* descriptorHeaps[] = { SrvManager::GetInstance()->GetDescriptorHeap().Get() };
	dxUtility_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void SkyboxCommon::Finalize() {

	delete instance;
	instance = nullptr;
}

void SkyboxCommon::CreateRootSignature() {
}

void SkyboxCommon::CreateInputLayout() {
}

void SkyboxCommon::CreateBlendState() {

	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}

void SkyboxCommon::CreateRasterizerState() {
}

void SkyboxCommon::CreateVertexShader() {
}

void SkyboxCommon::CreatePixelShader() {
}

void SkyboxCommon::CreateDepthStencilState() {
}

void SkyboxCommon::CreateGraphicsPipeline() {
}

SkyboxCommon* SkyboxCommon::GetInstance() {
	return nullptr;
}

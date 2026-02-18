#include "LineRenderer.h"
#include "DirectXUtility.h"
#include "SrvManager.h"

using namespace Engine;

void LineRenderer::Initialize() {

	// DirectXユーティリティのインスタンス取得
	dxUtility_ = DirectXUtility::GetInstance();

	// Srvマネージャのインスタンス取得
	srvManager_ = SrvManager::GetInstance();

	// ルートシグネチャ作成
	CreateRootSignature();

	// ルートシグネチャをパイプラインに設定
	pipelineCreator_.SetRootSignature(rootSignature_.Get());

	// インプットレイアウト作成
	CreateInputLayout();

	// インプットレイアウトをパイプラインに設定
	pipelineCreator_.SetInputLayoutDesc(inputLayoutDesc);

	// シェーダーをパイプラインに設定
	pipelineCreator_.SetVSFileName(L"");
	pipelineCreator_.SetPSFileName(L"");

	// パイプライン作成
	pipelineCreator_.Create();
}

void LineRenderer::SettingDrawing() {

	ID3D12GraphicsCommandList* commandList = dxUtility_->GetCommandList().Get();

	// ルートシグネチャを設定
	commandList->SetGraphicsRootSignature(rootSignature_.Get());

	// パイプラインを設定
	commandList->SetPipelineState(pipelineCreator_.GetGraphicsPipelineState().Get());

	// プリミティブトポロジーを三角形で設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvManager_->GetDescriptorHeap().Get() }; // ディスクリプタヒープ
	commandList->SetDescriptorHeaps(1, descriptorHeaps);
}

void LineRenderer::Finalize() {

	delete instance_;
	instance_ = nullptr;
}

void LineRenderer::CreateRootSignature() {

	D3D12_ROOT_SIGNATURE_DESC 
}

void LineRenderer::CreateInputLayout() {
}

LineRenderer* LineRenderer::instance_ = nullptr;

LineRenderer* LineRenderer::GetInstance() {
	
	if (instance_ == nullptr) {
		instance_ = new LineRenderer;
	}
	return instance_;
}

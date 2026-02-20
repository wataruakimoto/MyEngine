#include "LineRenderer.h"
#include "DirectXUtility.h"
#include "Logger.h"

using namespace Engine;
using namespace Logger;
using namespace Microsoft::WRL;

void LineRenderer::Initialize() {

	// DirectXユーティリティのインスタンス取得
	dxUtility_ = DirectXUtility::GetInstance();

	// ルートシグネチャ作成
	CreateRootSignature();

	// ルートシグネチャをパイプラインに設定
	pipelineCreator_.SetRootSignature(rootSignature_.Get());

	// インプットレイアウト作成
	CreateInputLayout();

	// インプットレイアウトをパイプラインに設定
	pipelineCreator_.SetInputLayoutDesc(inputLayoutDesc);

	// シェーダーをパイプラインに設定
	pipelineCreator_.SetVSFileName(vertexShaderFileName);
	pipelineCreator_.SetPSFileName(pixelShaderFileName);

	// パイプライン作成
	pipelineCreator_.Create(BlendMode::AlphaBlend, CullMode::None, DepthMode::ReadWrite);
}

void LineRenderer::SettingDrawing() {

	ID3D12GraphicsCommandList* commandList = dxUtility_->GetCommandList().Get();

	// ルートシグネチャを設定
	commandList->SetGraphicsRootSignature(rootSignature_.Get());

	// パイプラインを設定
	commandList->SetPipelineState(pipelineCreator_.GetGraphicsPipelineState().Get());

	// プリミティブトポロジーを線で設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void LineRenderer::Finalize() {

	delete instance_;
	instance_ = nullptr;
}

void LineRenderer::CreateRootSignature() {

	// ルートシグネチャを生成する
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータの設定 1つ
	D3D12_ROOT_PARAMETER rootParameters[1] = {};

	// gTransformationMatrix CBV b0
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBufferを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0; // 0番のレジスタを使う

	rootSignatureDesc.pParameters = rootParameters;
	rootSignatureDesc.NumParameters = _countof(rootParameters);

	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	// シリアライズしてバイナリにする
	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	// バイナリ化できなかったら
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// バイナリを元にルートシグネチャを生成
	hr = dxUtility_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));

	// 成功したか確認
	assert(SUCCEEDED(hr));
}

void LineRenderer::CreateInputLayout() {

	// POSITION0 float4
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	// COLOR0 float4
	inputElementDescs[1].SemanticName = "COLOR";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	// インプットレイアウトに設定
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);
}

LineRenderer* LineRenderer::instance_ = nullptr;

LineRenderer* LineRenderer::GetInstance() {
	
	if (instance_ == nullptr) {
		instance_ = new LineRenderer;
	}
	return instance_;
}

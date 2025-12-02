#include "SkyboxCommon.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "Logger.h"

using namespace Microsoft::WRL;
using namespace Logger;

void SkyboxCommon::Initialize() {

	// RootSignatureを生成
	CreateRootSignature();

	// RootSignatureをセット
	pipelineCreater_.SetRootSignature(rootSignature.Get());

	// InputLayoutを生成
	CreateInputLayout();

	// InputLayoutをセット
	pipelineCreater_.SetInputLayoutDesc(inputLayoutDesc);

	// シェーダパスをセット
	pipelineCreater_.SetVSFileName(L"Skybox.VS.hlsl");
	pipelineCreater_.SetPSFileName(L"Skybox.PS.hlsl");

	// パイプライン生成
	pipelineCreater_.Create(BlendMode::None, CullMode::None, DepthMode::ReadOnly);
}

void SkyboxCommon::SettingDrawing() {

	/// === ルートシグネチャをセットするコマンド === ///
	dxUtility_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	/// === グラフィックスパイプラインステートをセットするコマンド === ///
	dxUtility_->GetCommandList()->SetPipelineState(pipelineCreater_.GetGraphicsPipelineState().Get());

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

	// RootSignatureを生成する
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRangeを作成
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	// gTransformation CBV t0
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号0を使う

	// gMaterial CBV b0
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0; // レジスタ番号0を使う

	// gTexture SRV t0
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

	descriptionRootSignature.pParameters = rootParameters; // ルートパラメータ配列のポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters); // 配列の長さ

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// シリアライズしてバイナリにする
	ComPtr <ID3DBlob> signatureBlob = nullptr;
	ComPtr <ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// バイナリを元に生成
	hr = dxUtility_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
}

void SkyboxCommon::CreateInputLayout() {

	// InputLayoutの設定を行う
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);
}

SkyboxCommon* SkyboxCommon::instance = nullptr;

SkyboxCommon* SkyboxCommon::GetInstance() {
	
	if (instance == nullptr) {
		instance = new SkyboxCommon;
	}
	return instance;
}

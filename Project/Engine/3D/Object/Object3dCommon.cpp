#include "Object3dCommon.h"
#include "SrvManager.h"
#include "Logger.h"
#include "DebugCamera.h"

using namespace Microsoft::WRL;
using namespace Logger;

Object3dCommon* Object3dCommon::instance = nullptr;

Object3dCommon* Object3dCommon::GetInstance() {

	if (instance == nullptr) {
		instance = new Object3dCommon;
	}
	return instance;
}

void Object3dCommon::Initialize() {

	// RootSignatureを生成
	CreateRootSignature();

	// RootSignatureをセット
	pipelineCreater_.SetRootSignature(rootSignature.Get());

	// InputLayoutを生成
	CreateInputLayout();

	// InputLayoutをセット
	pipelineCreater_.SetInputLayoutDesc(inputLayoutDesc);

	// シェーダパスをセット
	pipelineCreater_.SetVSFileName(L"Object3d.VS.hlsl");
	pipelineCreater_.SetPSFileName(L"Object3d.PS.hlsl");

	// パイプライン生成
	pipelineCreater_.Create(Preset::Object3D);
}

void Object3dCommon::Finalize() {
	delete instance;
	instance = nullptr;
}

void Object3dCommon::SettingDrawing() {

	/// === ルートシグネチャをセットするコマンド === ///
	dxUtility_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	/// === グラフィックスパイプラインステートをセットするコマンド === ///
	dxUtility_->GetCommandList()->SetPipelineState(pipelineCreater_.GetGraphicsPipelineState().Get());

	/// === プリミティブトポロジーをセットするコマンド === ///
	dxUtility_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D12DescriptorHeap* descriptorHeaps[] = { SrvManager::GetInstance()->GetDescriptorHeap().Get() };
	dxUtility_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void Object3dCommon::CreateRootSignature() {

	// RootSignatureを生成する
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DescriptorRangeを作成
	D3D12_DESCRIPTOR_RANGE descriptorRange[2] = {};

	// gTexture SRV t0
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// gEnvironmentTexture SRV t1
	descriptorRange[1].BaseShaderRegister = 1; // 1から始まる
	descriptorRange[1].NumDescriptors = 1; // 数は1つ
	descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成。PixelShaderのMaterialとVertexShaderのTransform
	D3D12_ROOT_PARAMETER rootParameters[9] = {};

	// gMaterial CBV b0
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号0を使う

	// gTransformationMatrix CBV b6
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 6; // レジスタ番号0を使う

	// gTexture SRV t0
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = &descriptorRange[0]; // Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数

	// gEnvironmentTexture SRV t1
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = &descriptorRange[1]; // Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数

	// gDirectionalLight CBV b1
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[4].Descriptor.ShaderRegister = 1; // レジスタ番号1を使う

	// gPointLight CBV b2
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[5].Descriptor.ShaderRegister = 2; // レジスタ番号2を使う

	// gSpotLight CBV b3
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[6].Descriptor.ShaderRegister = 3; // レジスタ番号3を使う

	// gSpotLight CBV b4
	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[7].Descriptor.ShaderRegister = 4; // レジスタ番号3を使う

	// gCamera CBV b5
	rootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[8].Descriptor.ShaderRegister = 5; // レジスタ番号4を使う

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

void Object3dCommon::CreateInputLayout() {

	// InputLayoutの設定を行う
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	// InputLayoutに設定を
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);
}


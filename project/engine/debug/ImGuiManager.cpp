#include "ImGuiManager.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "winApp/WinApp.h"

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* dxCommon) {

	// 引数をメンバ変数にコピー
	dxCommon_ = dxCommon;

	// バージョンチェック
	IMGUI_CHECKVERSION();

	// コンテキストの生成
	ImGui::CreateContext();

	// スタイルの設定
	ImGui::StyleColorsDark();

	// Win32用の初期化
	ImGui_ImplWin32_Init(winApp->GetHwnd());

	/// === デスクリプタヒープ設定 === ///
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	/// === デスクリプタヒープ生成 === ///
	HRESULT hr = dxCommon->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(hr));

	// DirectX12用の設定
	ImGui_ImplDX12_Init(
		dxCommon->GetDevice().Get(),
		dxCommon->GetBackBufferCount(),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);
}

void ImGuiManager::Begin() {

	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End() {

	// ImGuiの内部コマンドを生成する
	ImGui::Render();
}

void ImGuiManager::Draw() {

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList().Get();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

#ifdef _DEBUG

	// 実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

#endif // _DEBUG

}

void ImGuiManager::Finalize() {

	// ImGuiの終了処理。初期化と逆順に行う
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// デスクリプタヒープ解放
	srvHeap_.Reset();
}
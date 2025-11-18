#include "ImGuiManager.h"
#include "WinApp.h"
#include "DirectXUtility.h"
#include "SwapChain.h"
#include "SrvManager.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

void ImGuiManager::Initialize(WinApp* winApp, SwapChain* swapChain) {

	// 引数をメンバ変数にコピー
	swapChain_ = swapChain;

	// DirectXUtilityのインスタンスを取得
	dxUtility_ = DirectXUtility::GetInstance();

	// バージョンチェック
	IMGUI_CHECKVERSION();

	// コンテキストの生成
	ImGui::CreateContext();

	// スタイルの設定
	ImGui::StyleColorsDark();

	// Win32用の初期化
	ImGui_ImplWin32_Init(winApp->GetHwnd());

	// SRVを確保してインデックスを得る
	uint32_t srvIndex = SrvManager::GetInstance()->Allocate();

	// DirectX12用の設定
	ImGui_ImplDX12_Init(
		dxUtility_->GetDevice().Get(),
		swapChain_->GetBackBufferCount(),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		SrvManager::GetInstance()->GetDescriptorHeap().Get(),
		SrvManager::GetInstance()->GetCPUDescriptorHandle(srvIndex),
		SrvManager::GetInstance()->GetGPUDescriptorHandle(srvIndex)
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

	ID3D12GraphicsCommandList* commandList = dxUtility_->GetCommandList().Get();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { SrvManager::GetInstance()->GetDescriptorHeap().Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

void ImGuiManager::Finalize() {

#ifdef USE_IMGUI

	// ImGuiの終了処理。初期化と逆順に行う
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

#endif // USE_IMGUI

	delete instance;
	instance = nullptr;
}

ImGuiManager* ImGuiManager::instance = nullptr;

ImGuiManager* ImGuiManager::GetInstance() {

	if (instance == nullptr) {
		instance = new ImGuiManager;
	}
	return instance;
}

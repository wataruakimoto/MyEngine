#include "ImGuiManager.h"
#include "WinApp.h"
#include "DirectXUtility.h"
#include "SwapChain.h"
#include "SrvManager.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

void ImGuiManager::Initialize(WinApp* winApp, SwapChain* swapChain) {

#ifdef USE_IMGUI

	// 引数をメンバ変数にコピー
	swapChain_ = swapChain;

	// DirectXUtilityのインスタンスを取得
	dxUtility_ = DirectXUtility::GetInstance();

	// バージョンチェック
	IMGUI_CHECKVERSION();

	// コンテキストの生成
	ImGui::CreateContext();

	// IO設定の取得
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// ドッキングを有効化
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// フォントのフルパスを作成
	std::string fontFullPath = fontResourcePath_ + fontFileName_;

	// フォントの読み込み
	io.Fonts->AddFontFromFileTTF(
		fontFullPath.c_str(), // フルパスから指定
		13.0f, // フォントサイズ
		nullptr,
		io.Fonts->GetGlyphRangesJapanese() // 日本語対応
	);

	// フォントアトラスのビルドを明示的に呼ぶ
	io.Fonts->Build();

	ImGuiStyle& style = ImGui::GetStyle();

	// 丸みの調整 (数値を大きくすると丸くなる)
	style.WindowRounding = 5.0f;    // ウィンドウの角丸
	style.FrameRounding = 4.0f;     // 入力欄やボタンの角丸
	style.GrabRounding = 4.0f;      // スライダーのツマミの角丸
	style.PopupRounding = 4.0f;     // ポップアップの角丸
	style.ChildRounding = 4.0f;

	// 余白の調整
	style.WindowPadding = ImVec2(10, 10);
	style.FramePadding = ImVec2(5, 5);
	style.ItemSpacing = ImVec2(8, 6);

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

#endif // USE_IMGUI
}

void ImGuiManager::Begin() {

#ifdef USE_IMGUI

	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ドッキングスペースの作成
	ImGui::DockSpaceOverViewport(
		ImGui::GetMainViewport()->ID,
		ImGui::GetMainViewport(),
		ImGuiDockNodeFlags_PassthruCentralNode
	);

#endif // USE_IMGUI
}

void ImGuiManager::End() {

#ifdef USE_IMGUI

	// ImGuiの内部コマンドを生成する
	ImGui::Render();

#endif // USE_IMGUI
}

void ImGuiManager::Draw() {

#ifdef USE_IMGUI

	ID3D12GraphicsCommandList* commandList = dxUtility_->GetCommandList().Get();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { SrvManager::GetInstance()->GetDescriptorHeap().Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

#endif // USE_IMGUI
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

#include "LightManager.h"

#include "DirectXUtility.h"

#include <numbers>
#include <string>
#include <imgui.h>

using namespace Engine;

void LightManager::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility_ = DirectXUtility::GetInstance();

	// 平行光源を作成
	CreateDirectionalLight();

	// 点光源を作成
	CreatePointLight();

	// スポットライトを作成
	CreateSpotLight();

	// 環境マップを作成
	CreateEnvironmentMap();
}

void LightManager::Update() {
}

void LightManager::Draw() {

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = dxUtility_->GetCommandList().Get();

	/// === 平行光源CBufferの場所を設定 === ///
	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	/// === 点光源CBufferの場所を設定 === ///
	commandList->SetGraphicsRootConstantBufferView(4, pointLightResource_->GetGPUVirtualAddress());

	/// === スポットライトCBufferの場所を設定 === ///
	commandList->SetGraphicsRootConstantBufferView(5, spotLightResource_->GetGPUVirtualAddress());

	/// === 環境マップCBufferの場所を設定 === ///
	commandList->SetGraphicsRootConstantBufferView(6, environmentMapResource_->GetGPUVirtualAddress());
}

void LightManager::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("光源マネージャ");

	if (ImGui::TreeNodeEx("平行光源")) {

		//for (uint32_t i = 0; i < MAX_DIR_LIGHTS; i++) {
		//
		//	if (ImGui::TreeNodeEx(std::to_string(i).c_str())) {
		//
		//		ImGui::DragFloat3("向き", &directionalLightData_->lights[i].direction.x, 0.01f);
		//		ImGui::DragFloat("輝度", &directionalLightData_->lights[i].intensity, 0.01f);
		//		ImGui::ColorEdit4("色", &directionalLightData_->lights[i].color.x);
		//
		//		ImGui::TreePop();
		//	}
		//}

		ImGui::DragFloat3("向き", &directionalLightData_->direction.x, 0.01f);
		ImGui::DragFloat("輝度", &directionalLightData_->intensity, 0.01f);
		ImGui::ColorEdit4("色", &directionalLightData_->color.x);

		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("点光源")) {

		ImGui::DragFloat3("位置", &pointLightData_->position.x, 0.01f);
		ImGui::DragFloat("輝度", &pointLightData_->intensity, 0.01f);
		ImGui::DragFloat("距離", &pointLightData_->distance, 0.01f);
		ImGui::DragFloat("減衰率", &pointLightData_->decay, 0.01f);
		ImGui::ColorEdit4("色", &pointLightData_->color.x);

		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("スポットライト")) {

		ImGui::DragFloat3("位置", &spotLightData_->position.x, 0.01f);
		ImGui::DragFloat3("向き", &spotLightData_->direction.x, 0.01f);
		ImGui::DragFloat("輝度", &spotLightData_->intensity, 0.01f);
		ImGui::DragFloat("距離", &spotLightData_->distance, 0.01f);
		ImGui::DragFloat("減衰率", &spotLightData_->decay, 0.01f);
		ImGui::DragFloat("角度", &spotLightData_->cosAngle, 0.01f);
		ImGui::DragFloat("減衰開始角度", &spotLightData_->cosFalloffStart, 0.01f);
		ImGui::ColorEdit4("色", &spotLightData_->color.x);

		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("環境マップ")) {

		ImGui::DragFloat("輝度", &environmentMapData_->intensity, 0.01f);

		ImGui::TreePop();
	}

	ImGui::End();

#endif // USE_IMGUI
}

void LightManager::CreateDirectionalLight() {

	// 平行光源リソースを作成
	directionalLightResource_ = dxUtility_->CreateBufferResource(sizeof(DirectionalLight));

	// VRAMにマップして、データを書き込むためのアドレスを取得して、割り当てる
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));

	// 平行光源データの初期値を書き込む
	directionalLightData_->direction = { 0.0f, -1.0f, 0.0f }; // 下向き
	directionalLightData_->intensity = 1.0f; // 輝度は最大
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 色は白
	
	//directionalLightData_->activeCount = 1; // 1つだけ有効にしておく
	//
	//// 0番目(メイン)の初期化
	//directionalLightData_->lights[0].direction = { 0.0f, -1.0f, 0.0f }; // 下向き
	//directionalLightData_->lights[0].intensity = 1.0f; // 輝度は最大
	//directionalLightData_->lights[0].color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 色は白
	//
	//// 1番目以降の初期化
	//for (uint32_t i = 1; i < MAX_DIR_LIGHTS; i++) {
	//
	//	directionalLightData_->lights[i].direction = { 0.0f, -1.0f, 0.0f }; // 下向き
	//	directionalLightData_->lights[i].intensity = 0.0f; // 輝度を0
	//	directionalLightData_->lights[i].color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 色は白
	//}
}

void LightManager::CreatePointLight() {

	// 点光源リソースを作成
	pointLightResource_ = dxUtility_->CreateBufferResource(sizeof(PointLight));

	// VRAMにマップして、データを書き込むためのアドレスを取得して、割り当てる
	pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));

	// 点光源データの初期値を書き込む
	pointLightData_->position = { 0.0f, 0.0f, 0.0f }; // 位置は原点
	pointLightData_->intensity = 1.0f; // 輝度は最大
	pointLightData_->distance = 10.0f; // 光の届く最大距離
	pointLightData_->decay = 2.0f; // 減衰率
	pointLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 色は白
}

void LightManager::CreateSpotLight() {

	// スポットライトリソースを作成
	spotLightResource_ = dxUtility_->CreateBufferResource(sizeof(SpotLight));

	// VRAMにマップして、データを書き込むためのアドレスを取得して、割り当てる
	spotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));

	// スポットライトデータの初期値を書き込む
	spotLightData_->position = { 0.0f, 0.0f, 0.0f }; // 位置は原点
	spotLightData_->direction = { 0.0f, -1.0f, 0.0f }; // 下向き
	spotLightData_->intensity = 1.0f; // 輝度は最大
	spotLightData_->distance = 10.0f; // 光の届く最大距離
	spotLightData_->decay = 2.0f; // 減衰率
	spotLightData_->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f); // π/3
	spotLightData_->cosFalloffStart = spotLightData_->cosAngle + 0.01f; // cosAngleよりちょっと大きい
	spotLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 色は白
}

void LightManager::CreateEnvironmentMap() {

	// 環境マップリソースを作成
	environmentMapResource_ = dxUtility_->CreateBufferResource(sizeof(EnvironmentMap));

	// VRAMにマップして、データを書き込むためのアドレスを取得して、割り当てる
	environmentMapResource_->Map(0, nullptr, reinterpret_cast<void**>(&environmentMapData_));

	// 環境マップデータの初期値を書き込む
	environmentMapData_->intensity = 1.0f; // 輝度は最大
}

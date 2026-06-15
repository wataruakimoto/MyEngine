#include "CameraManager.h"

#include <cassert>

using namespace Engine;

/// ================================================== ///
/// 初期化
void CameraManager::Initialize() {

	// カメラの生成
	camera_ = std::make_unique<Engine::Camera>();
	// カメラの初期化
	camera_->Initialize();
}

/// ================================================== ///
/// 更新
void CameraManager::Update() {

	// 現在のカメラコントローラーが存在する場合
	if (currentController_) {

		// 現在のカメラコントローラーを更新
		currentController_->Update();
	}

	// カメラの更新
	camera_->Update();
}

/// ================================================== ///
/// カメラコントローラーの切り替え
void CameraManager::AddCameraController(const std::string& name, std::unique_ptr<ICameraController> controller) {

	// コントローラーを追加
	cameraControllers_[name] = std::move(controller);

	// コントローラーにカメラを渡す
	cameraControllers_[name]->SetCamera(camera_.get());
}

/// ================================================== ///
/// カメラコントローラーの切り替え
void CameraManager::SwitchCameraController(const std::string& name) {

	// 次に使用するコントローラー
	ICameraController* nextController = nullptr;

	// マップをループ
	for (const auto& pair : cameraControllers_) {

		// 名前が一致するコントローラーを探す
		if (pair.first == name) {

			// 次に使用するコントローラーを保存
			nextController = pair.second.get();

			// ループを抜ける
			break;
		}
	}

	// 指定のコントローラーが見つからない場合は
	if (!nextController) {

		// エラー
		assert(false && "Camera controller not found");
		return;
	}

	// コントローラーを切り替える
	currentController_ = nextController;
	
	// カメラのワールド変換の親を、新しいコントローラーのワールド変換にする
	camera_->GetWorldTransform().SetParent(&currentController_->GetWorldTransform());

	// カメラのワールド変換をリセット
	camera_->GetWorldTransform().SetRotate(Vector3{ 0.0f, 0.0f, 0.0f });
	camera_->GetWorldTransform().SetTranslate({ 0.0f, 0.0f, 0.0f });
	camera_->Update();
}

/// ================================================== ///
/// ワールド全体をずらす
void CameraManager::ShiftWorld(float shiftZ) {

	// カメラを手前にずらす
	currentController_->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
}

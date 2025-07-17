#include "RailCameraController.h"
#include "Camera/Camera.h"

void RailCameraController::Initialize() {
	
	// ワールド変換の初期化
	worldTransform.Initialize();
}

void RailCameraController::Update() {
	
	// ワールド変換の更新
	worldTransform.Update();
}
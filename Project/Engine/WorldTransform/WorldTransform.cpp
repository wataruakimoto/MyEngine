#include "WorldTransform.h"
#include "Base/DirectXUtility.h"
#include "Camera/Camera.h"
#include "Math/MathMatrix.h"

using namespace MathMatrix;

void WorldTransform::Initialize() {

	// リソース作成
	//transformationResource_ = dxUtility_->CreateBufferResource(sizeof(Transformation));

	// リソースにデータを書き込むためのアドレスを取得、割り当てする
	//transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData_));

	// 座標変換データの初期化
	transformationData_.worldMatrix = MakeIdentity4x4(); // 単位行列を書き込む
	transformationData_.WVPMatrix = MakeIdentity4x4(); // 単位行列を書き込む
}

void WorldTransform::UpdateMatrix() {

	// ワールド行列を作成
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);

	// 親が割り当てられていたら
	if (parent_) {

		// 親のワールド行列を掛け合わせる
		worldMatrix_ *= parent_->worldMatrix_;
	}

	// カメラが割り当てられていたら
	if (camera_) {

		// ビュープロジェクション行列を取得
		Matrix4x4 viewProjectionMatrix = camera_->GetViewProjectionMatrix();

		// ワールド行列とビュープロジェクション行列を掛け合わせてWVP行列を作成
		WVPMatrix_ = worldMatrix_ * viewProjectionMatrix;

	}
	else {

		// WVP行列はワールド行列と同じにしておく
		WVPMatrix_ = worldMatrix_;
	}
}

void WorldTransform::TransferMatrix() {

	// 座標変換データに書き込む
	transformationData_.worldMatrix = worldMatrix_;
	transformationData_.WVPMatrix = WVPMatrix_;

	// コマンドリストにリソースを転送
	dxUtility_->GetCommandList()->SetComputeRootShaderResourceView(1, transformationResource_->GetGPUVirtualAddress());
}

void WorldTransform::Update() {

	// 行列の更新
	UpdateMatrix();

	// 行列の転送
	TransferMatrix();
}
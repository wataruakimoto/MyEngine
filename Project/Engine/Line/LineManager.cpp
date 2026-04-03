#include "LineManager.h"
#include "DirectXUtility.h"
#include "Camera.h"
#include "MathMatrix.h"

#include <cassert>
#include <numbers>

using namespace Engine;
using namespace MathMatrix;

void LineManager::Initialize() {

	// DirectXユーティリティのインスタンス取得
	dxUtility_ = DirectXUtility::GetInstance();

	// 頂点バッファの生成
	GenerateVertexBuffer();

	// 座標変換バッファの生成
	GenerateTranformationBuffer();

	// 頂点データの初期化 メモリ確保
	vertexDatas_.reserve(kMaxVertices_);
}

void LineManager::Clear() {

	// 頂点データのクリア
	vertexDatas_.clear();
}

void LineManager::DrawLine(const Vector3& start, const Vector3& end, const Vector4& color) {

	// 頂点数の確認 2点追加して最大数を超えたら
	if (vertexDatas_.size() + 2 > kMaxVertices_) {

		assert(false && "頂点数が最大数を超えました");
		return;
	}

	/// ===== 開始点 ===== ///

	// 開始の頂点データ
	VertexData startVertex;
	startVertex.position = { start.x, start.y, start.z, 1.0f }; // w成分は1.0f
	startVertex.color = color;

	// 配列に追加
	vertexDatas_.push_back(startVertex);

	/// ===== 終了点 ===== ///

	// 終了の頂点データ
	VertexData endVertex;
	endVertex.position = { end.x, end.y, end.z, 1.0f }; // w成分は1.0f
	endVertex.color = color;

	// 配列に追加
	vertexDatas_.push_back(endVertex);
}

void LineManager::DrawSphere(const Vector3& center, float radius, const uint32_t subdivision, const Vector4& color) {

	// π
	const float pi = std::numbers::pi_v<float>;

	// 1分割あたりの角度
	const float kLonEvery = pi * 2.0f / static_cast<float>(subdivision);
	const float kLatEvery = pi / static_cast<float>(subdivision);

	// 全頂点を生成
	for (uint32_t latIndex = 0; latIndex < subdivision; ++latIndex) {

		float lat = -pi / 2.0f + kLatEvery * latIndex;
		float nextLat = lat + kLatEvery;

		for (uint32_t lonIndex = 0; lonIndex < subdivision; ++lonIndex) {

			float lon = lonIndex * kLonEvery;
			float nextLon = (lonIndex + 1) * kLonEvery;

			/// ========== 現在の点 ========== ///

			Vector3 a = {
				std::cos(lat) * std::cos(lon) * radius,
				std::sin(lat) * radius,
				std::cos(lat) * std::sin(lon) * radius,
			};

			// 中心座標を加算
			a += center;

			/// ========== 隣の横の点 ========== ///

			Vector3 b = {
				std::cos(lat) * std::cos(nextLon) * radius,
				std::sin(lat) * radius,
				std::cos(lat) * std::sin(nextLon) * radius,
			};

			// 中心座標を加算
			b += center;

			/// ========== 隣の縦の点 ========== ///

			Vector3 c = {
				std::cos(nextLat) * std::cos(lon) * radius,
				std::sin(nextLat) * radius,
				std::cos(nextLat) * std::sin(lon) * radius,
			};

			// 中心座標を加算
			c += center;

			/// ========== 線を引く ========== ///

			// 横線
			DrawLine(a, b, color);

			// 縦線
			DrawLine(a, c, color);
		}
	}
}

void LineManager::DrawSphere(const Sphere& sphere, const uint32_t subdivision, const Vector4& color) {

	DrawSphere(sphere.center, sphere.radius, subdivision, color);
}

void LineManager::DrawAABB(const Vector3& min, const Vector3& max, const Vector4& color) {

	// 8頂点を定義
	Vector3 vertices[8] = {
		{ min.x, min.y, min.z }, // 0: 最小点
		{ max.x, min.y, min.z }, // 1: x最大
		{ max.x, max.y, min.z }, // 2: x最大、y最大
		{ min.x, max.y, min.z }, // 3: y最大
		{ min.x, min.y, max.z }, // 4: z最大
		{ max.x, min.y, max.z }, // 5: x最大、z最大
		{ max.x, max.y, max.z }, // 6: x最大、y最大、z最大
		{ min.x, max.y, max.z }  // 7: y最大、z最大
	};

	// 12辺を描画

	DrawLine(vertices[0], vertices[1], color); // 下辺1
	DrawLine(vertices[1], vertices[2], color); // 下辺2
	DrawLine(vertices[2], vertices[3], color); // 下辺3
	DrawLine(vertices[3], vertices[0], color); // 下辺4

	DrawLine(vertices[4], vertices[5], color); // 上辺1
	DrawLine(vertices[5], vertices[6], color); // 上辺2
	DrawLine(vertices[6], vertices[7], color); // 上辺3
	DrawLine(vertices[7], vertices[4], color); // 上辺4

	DrawLine(vertices[0], vertices[4], color); // 垂直辺1
	DrawLine(vertices[1], vertices[5], color); // 垂直辺2
	DrawLine(vertices[2], vertices[6], color); // 垂直辺3
	DrawLine(vertices[3], vertices[7], color); // 垂直辺4
}

void LineManager::DrawAABB(const AABB& aabb, const Vector4& color) {

	DrawAABB(aabb.min, aabb.max, color);
}

void LineManager::DrawOBB(const Vector3& center, const Vector3 orientations[3], const Vector3& halfSize, const Vector4& color) {

	// 中心から面までの距離ベクトルを計算
	Vector3 axes[3] = {
		orientations[0] * halfSize.x,
		orientations[1] * halfSize.y,
		orientations[2] * halfSize.z
	};

	// 8頂点を定義
	Vector3 vertices[8] = {
		center - axes[0] + axes[1] - axes[2], // 左上前
		center + axes[0] + axes[1] - axes[2], // 右上前
		center + axes[0] - axes[1] - axes[2], // 右下前
		center - axes[0] - axes[1] - axes[2], // 左下前
		center - axes[0] + axes[1] + axes[2], // 左上奥
		center + axes[0] + axes[1] + axes[2], // 右上奥
		center + axes[0] - axes[1] + axes[2], // 右下奥
		center - axes[0] - axes[1] + axes[2], // 左下奥
	};

	// 12辺を描画

	// 手前の面
	DrawLine(vertices[0], vertices[1], color);
	DrawLine(vertices[1], vertices[2], color);
	DrawLine(vertices[2], vertices[3], color);
	DrawLine(vertices[3], vertices[0], color);

	// 奥の面
	DrawLine(vertices[4], vertices[5], color);
	DrawLine(vertices[5], vertices[6], color);
	DrawLine(vertices[6], vertices[7], color);
	DrawLine(vertices[7], vertices[4], color);

	// 手前と奥を繋ぐ辺
	DrawLine(vertices[0], vertices[4], color);
	DrawLine(vertices[1], vertices[5], color);
	DrawLine(vertices[2], vertices[6], color);
	DrawLine(vertices[3], vertices[7], color);
}

void LineManager::DrawOBB(const OBB& obb, const Vector4& color) {

	DrawOBB(obb.center, obb.orientations, obb.halfSize, color);
}

void LineManager::DrawEllipsoid(const Vector3& center, const Vector3& radius, const uint32_t subdivision, const Vector4& color) {

	// π
	const float pi = std::numbers::pi_v<float>;

	// 1分割あたりの角度
	const float kLonEvery = pi * 2.0f / static_cast<float>(subdivision);
	const float kLatEvery = pi / static_cast<float>(subdivision);

	// 全頂点を生成
	for (uint32_t latIndex = 0; latIndex < subdivision; ++latIndex) {

		float lat = -pi / 2.0f + kLatEvery * latIndex;
		float nextLat = lat + kLatEvery;

		for (uint32_t lonIndex = 0; lonIndex < subdivision; ++lonIndex) {

			float lon = lonIndex * kLonEvery;
			float nextLon = (lonIndex + 1) * kLonEvery;

			/// ========== 現在の点 ========== ///

			Vector3 a = {
				std::cos(lat) * std::cos(lon) * radius.x,
				std::sin(lat) * radius.y,
				std::cos(lat) * std::sin(lon) * radius.z,
			};

			// 中心座標を加算
			a += center;

			/// ========== 隣の横の点 ========== ///

			Vector3 b = {
				std::cos(lat) * std::cos(nextLon) * radius.x,
				std::sin(lat) * radius.y,
				std::cos(lat) * std::sin(nextLon) * radius.z,
			};

			// 中心座標を加算
			b += center;

			/// ========== 隣の縦の点 ========== ///

			Vector3 c = {
				std::cos(nextLat) * std::cos(lon) * radius.x,
				std::sin(nextLat) * radius.y,
				std::cos(nextLat) * std::sin(lon) * radius.z,
			};

			// 中心座標を加算
			c += center;

			/// ========== 線を引く ========== ///

			// 横線
			DrawLine(a, b, color);

			// 縦線
			DrawLine(a, c, color);
		}
	}
}

void LineManager::DrawEllipsoid(const Ellipsoid& ellipsoid, const uint32_t subdivision, const Vector4& color) {

	DrawEllipsoid(ellipsoid.center, ellipsoid.radius, subdivision, color);
}

void LineManager::DrawGrid(const Vector3& center, const float size, const uint32_t subdivision, const Vector4& color) {

	float halfSize = size / 2.0f;

	float step = size / static_cast<float>(subdivision);

	for (uint32_t i = 0; i <= subdivision; ++i) {

		// 中心からのオフセット
		float offset = -halfSize + (i * step);

		/// ========== 縦線 ========== ///

		Vector3 startX = { center.x - halfSize, center.y, center.z + offset };
		Vector3 endX = { center.x + halfSize, center.y, center.z + offset };
		DrawLine(startX, endX, color);

		/// ========== 横線 ========== ///

		Vector3 startZ = { center.x + offset, center.y, center.z - halfSize };
		Vector3 endZ = { center.x + offset, center.y, center.z + halfSize };
		DrawLine(startZ, endZ, color);
	}
}

void LineManager::Render() {

	// 頂点データがなければ描画しない
	if (vertexDatas_.empty()) return;

	// カメラからビュープロジェクション行列を取得して座標変換データにセット
	transformationData_->viewProjection = defaultCamera_->GetViewProjectionMatrix();

	// アドレスを受け取る用のポインタ
	void* mappedData = nullptr;

	// VRAMにデータを書き込むためにアドレスを取得
	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData));

	// 頂点データをVRAMに転送
	std::memcpy(mappedData, vertexDatas_.data(), sizeof(VertexData) * vertexDatas_.size());

	// マップ解除
	vertexBuffer_->Unmap(0, nullptr);

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxUtility_->GetCommandList().Get();

	// 頂点バッファビューを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// 座標変換バッファのCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, transformationBuffer_->GetGPUVirtualAddress());

	// 描画
	commandList->DrawInstanced(static_cast<UINT>(vertexDatas_.size()), 1, 0, 0);
}

void LineManager::Finalize() {

	delete instance_;
	instance_ = nullptr;
}

void LineManager::GenerateVertexBuffer() {

	// 頂点バッファを作成
	vertexBuffer_ = dxUtility_->CreateBufferResource(sizeof(VertexData) * kMaxVertices_);

	/// ===== 頂点バッファビューを作成 ===== ///

	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは最大頂点数分
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * kMaxVertices_;
	// 1頂点あたりのサイズはVertexData構造体のサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void LineManager::GenerateTranformationBuffer() {

	// 座標変換バッファを作成
	transformationBuffer_ = dxUtility_->CreateBufferResource(sizeof(TransformationData));

	// VRAMにデータを書き込むためにアドレスを取得
	transformationBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData_));

	// 単位行列で初期化する
	transformationData_->viewProjection = MakeIdentity4x4();
}

LineManager* LineManager::instance_ = nullptr;

LineManager* LineManager::GetInstance() {

	if (instance_ == nullptr) {
		instance_ = new LineManager;
	}
	return instance_;
}

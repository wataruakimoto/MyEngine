#include "LineManager.h"
#include "DirectXUtility.h"
#include "Camera.h"
#include "MathVector.h"
#include "MathMatrix.h"

#include <cassert>
#include <numbers>

using namespace Engine;
using namespace MathVector;
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

void LineManager::DrawCapsule(const Vector3& start, const Vector3& end, float radius, const uint32_t subdivision, const Vector4& color) {

	// 円柱の軸ベクトルと長さを計算
	Vector3 d = { end.x - start.x, end.y - start.y, end.z - start.z };
	float length = std::sqrt(d.x * d.x + d.y * d.y + d.z * d.z);

	// 描画用のローカル座標系（Y軸をカプセルの向きとする）を設定
	Vector3 localY = (length > 0.0001f) ? Vector3{ d.x / length, d.y / length, d.z / length } : Vector3{ 0, 1, 0 };
	Vector3 up = (std::abs(localY.y) < 0.999f) ? Vector3{ 0, 1, 0 } : Vector3{ 1, 0, 0 };

	// 外積 (Cross) と正規化 (Normalize) を用いて直交基底を作成
	Vector3 localX = Normalize(Cross(up, localY));
	Vector3 localZ = Normalize(Cross(localX, localY));

	const float PI = 3.14159265359f;

	// 赤道（phi=PI/2）を必ず計算できるように、極方向の分割数(rings)を偶数で保証する
	uint32_t rings = (subdivision % 2 == 0) ? subdivision : subdivision + 1;
	uint32_t slices = subdivision;
	uint32_t halfRings = rings / 2;

	// 球面の座標を計算し、指定した中心点(center)に対するワールド座標を返すラムダ
	auto GetSpherePoint = [&](float theta, float phi, const Vector3& center) -> Vector3 {
		// 標準的な球の座標計算
		float x = std::sin(phi) * std::cos(theta);
		float y = std::cos(phi);
		float z = std::sin(phi) * std::sin(theta);

		// カプセルの向きに合わせて回転
		Vector3 rotated = {
			localX.x * x + localY.x * y + localZ.x * z,
			localX.y * x + localY.y * y + localZ.y * z,
			localX.z * x + localY.z * y + localZ.z * z
		};

		// 中心点をオフセットしてスケール適用
		return {
			center.x + rotated.x * radius,
			center.y + rotated.y * radius,
			center.z + rotated.z * radius
		};
		};

	// 球と全く同じ2重ループ（緯度・経度）を利用する
	for (uint32_t i = 0; i < rings; ++i) {
		float phi1 = PI * (float)i / rings;
		float phi2 = PI * (float)(i + 1) / rings;

		// Phiの角度に応じて、上半球は end、下半球は start を中心にする
		Vector3 center1 = (i < halfRings) ? end : start;
		Vector3 center2 = (i + 1 <= halfRings) ? end : start;

		for (uint32_t j = 0; j < slices; ++j) {
			float theta1 = 2.0f * PI * (float)j / slices;
			float theta2 = 2.0f * PI * (float)(j + 1) / slices;

			Vector3 p0 = GetSpherePoint(theta1, phi1, center1);
			Vector3 p1 = GetSpherePoint(theta2, phi1, center1); // 水平リングの次点
			Vector3 p2 = GetSpherePoint(theta1, phi2, center2); // 垂直ラインの次点

			// 1. 水平方向のリング（緯線）線描画。一番上の極での縮退を防ぐため 0 より大きい場合のみ
			if (i > 0) {
				DrawLine(p0, p1, color);
			}

			// 2. 赤道（上半球と下半球の接続部分）における円柱側面の描画
			if (i == halfRings - 1) {
				// 上半球側の赤道リングを明示的に描画（通常の水平リングは phi1 に対して描かれるため）
				Vector3 equatorEnd0 = p2; // phi2 = PI/2 における end 側の点
				Vector3 equatorEnd1 = GetSpherePoint(theta2, phi2, center2);
				DrawLine(equatorEnd0, equatorEnd1, color);

				// 円柱の縦の側面ライン（上半球の赤道 〜 下半球の赤道）を描画
				Vector3 equatorStart0 = GetSpherePoint(theta1, phi2, start);
				DrawLine(equatorEnd0, equatorStart0, color);

				// ※ 下半球側の赤道リングは、次のループ(i == halfRings)の水平リング(phi1)として描画される
			}

			// 3. 垂直方向（経線）の線描画（上半球から下半球へ飛ぶ瞬間以外は通常通り描画）
			DrawLine(p0, p2, color);
		}
	}
}

void LineManager::DrawCapsule(const Capsule& capsule, const uint32_t subdivision, const Vector4& color) {

	DrawCapsule(capsule.start, capsule.end, capsule.radius, subdivision, color);
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

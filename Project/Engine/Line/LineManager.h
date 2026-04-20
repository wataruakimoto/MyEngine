#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Sphere.h"
#include "AABB.h"
#include "OBB.h"
#include "Capsule.h"
#include "Ellipsoid.h"

#include <d3d12.h>
#include <wrl.h>
#include <vector>

namespace Engine {

	/// ===== 前方宣言 ===== ///

	class DirectXUtility;
	class Camera;

	/// <summary>
	/// 線描画マネージャ
	/// </summary>
	class LineManager {

	/// ================================================== ///
	/// シングルトン
	/// ================================================== ///
	private:

		// インスタンス
		static LineManager* instance_;

		// コンストラクタ隠蔽
		LineManager() = default;
		// デストラクタ隠蔽
		~LineManager() = default;
		// コピーコンストラクタ禁止
		LineManager(LineManager&) = delete;
		// コピーオペレータ禁止
		LineManager& operator=(LineManager&) = delete;

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// フレーム開始時のクリア
		/// </summary>
		void Clear();

		/// <summary>
		/// 線描画
		/// </summary>
		/// <param name="start">始点</param>
		/// <param name="end">終点</param>
		/// <param name="color">色</param>
		void DrawLine(const Vector3& start, const Vector3& end, const Vector4& color);

		/// <summary>
		/// 球の線描画
		/// </summary>
		/// <param name="center">中心点</param>
		/// <param name="radius">半径</param>
		/// <param name="subdivision">分割数</param>
		/// <param name="color">色</param>
		void DrawSphere(const Vector3& center, float radius, const uint32_t subdivision, const Vector4& color);

		/// <summary>
		/// 球の線描画
		/// </summary>
		/// <param name="sphere">球</param>
		/// <param name="subdivision">分割数</param>
		/// <param name="color">色</param>
		void DrawSphere(const Sphere& sphere, const uint32_t subdivision, const Vector4& color);

		/// <summary>
		/// AABBの線描画
		/// </summary>
		/// <param name="min">最小</param>
		/// <param name="max">最大</param>
		/// <param name="color">色</param>
		void DrawAABB(const Vector3& min, const Vector3& max, const Vector4& color);

		/// <summary>
		/// AABBの線描画
		/// </summary>
		/// <param name="aabb">AABB</param>
		/// <param name="color">色</param>
		void DrawAABB(const AABB& aabb, const Vector4& color);

		/// <summary>
		/// OBBの線描画
		/// </summary>
		/// <param name="center">中心点</param>
		/// <param name="orientations">方向ベクトル</param>
		/// <param name="halfSize">中心から面までの距離</param>
		/// <param name="color">色</param>
		void DrawOBB(const Vector3& center, const Vector3 orientations[3], const Vector3& halfSize, const Vector4& color);

		/// <summary>
		/// OBBの線描画
		/// </summary>
		/// <param name="obb">OBB</param>
		/// <param name="color">色</param>
		void DrawOBB(const OBB& obb, const Vector4& color);

		/// <summary>
		/// カプセルの線描画
		/// </summary>
		/// <param name="start">始点</param>
		/// <param name="end">終点</param>
		/// <param name="radius">半径</param>
		/// <param name="subdivision">分割数</param>
		/// <param name="color">色</param>
		void DrawCapsule(const Vector3& start, const Vector3& end, float radius, const uint32_t subdivision, const Vector4& color);

		/// <summary>
		/// カプセルの線描画
		/// </summary>
		/// <param name="capsule">カプセル</param>
		/// <param name="subdivision">分割数</param>
		/// <param name="color">色</param>
		void DrawCapsule(const Capsule& capsule, const uint32_t subdivision, const Vector4& color);

		/// <summary>
		/// 楕円形の線描画
		/// </summary>
		/// <param name="center">中心点</param>
		/// <param name="radius">半径</param>
		/// <param name="subdivision">分割数</param>
		/// <param name="color">色</param>
		void DrawEllipsoid(const Vector3& center, const Vector3& radius, const uint32_t subdivision, const Vector4& color);

		/// <summary>
		/// 楕円形の線描画
		/// </summary>
		/// <param name="ellipsoid">楕円形</param>
		/// <param name="subdivision">分割数</param>
		/// <param name="color">色</param>
		void DrawEllipsoid(const Ellipsoid& ellipsoid, const uint32_t subdivision, const Vector4& color);

		/// <summary>
		/// グリッドの線描画
		/// </summary>
		/// <param name="center">中心点</param>
		/// <param name="size">グリッド全体の大きさ</param>
		/// <param name="subdivision">分割数</param>
		/// <param name="color">色</param>
		void DrawGrid(const Vector3& center, const float size, const uint32_t subdivision, const Vector4& color);

		/// <summary>
		/// 一括描画
		/// </summary>
		void Render();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
	private:

		/// <summary>
		/// 頂点データの生成
		/// </summary>
		void GenerateVertexBuffer();

		/// <summary>
		/// 座標変換データの生成
		/// </summary>
		void GenerateTranformationBuffer();

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns>インスタンス</returns>
		static LineManager* GetInstance();

	/// ================================================== ///
	/// セッター
	/// ================================================== ///
	public:

		/// <summary>
		/// デフォルトカメラの設定
		/// </summary>
		/// <param name="camera">カメラ</param>
		void SetDefaultCamera(Camera* camera) { defaultCamera_ = camera; }

	/// ================================================== ///
	/// 構造体
	/// ================================================== ///
	private:

		// 頂点データ
		struct VertexData {
			Vector4 position; // 座標
			Vector4 color; // 色
		};

		// 座標変換データ
		struct TransformationData {
			Matrix4x4 viewProjection; // ビュー射影行列
		};

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
	private:

		/// ===== GPU用の変数 ===== ///

		// 頂点バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_ = nullptr;

		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};

		// 頂点データ 動的配列
		std::vector<VertexData> vertexDatas_;

		// 最大頂点数 16Bit分
		static const size_t kMaxVertices_ = 65535;

		// 座標変換バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> transformationBuffer_ = nullptr;

		// 座標変換データ
		TransformationData* transformationData_ = nullptr;

		/// ===== 借りポインタ・インスタンス ===== ///

		// DirectXユーティリティのインスタンス
		DirectXUtility* dxUtility_ = nullptr;

		// デフォルトカメラ (カメラの借りポインタ)
		Camera* defaultCamera_ = nullptr;
	};
}


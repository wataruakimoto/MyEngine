#pragma once

#include "Vector3.h"
#include "Vector4.h"

#include <wrl.h>
#include <d3d12.h>

namespace Engine {

	/// ========== 前方宣言 ========== ///

	class DirectXUtility;

	/// <summary>
	/// 光源マネージャ
	/// </summary>
	class LightManager {

	/// ================================================== ///
	/// 構造体
	/// ================================================== ///
	public:

		/// <summary>
		/// 平行光源
		/// </summary>
		struct DirectionalLight {
			Vector3 direction; // 向き
			float intensity;   // 輝度
			Vector4 color;	   // 色
		};

		/// <summary>
		/// 点光源
		/// </summary>
		struct PointLight {
			Vector3 position; // 位置
			float intensity;  // 輝度
			float distance;   // 光の届く最大距離
			float decay;      // 減衰率
			float padding[2]; // パディング
			Vector4 color;    // 色
		};

		/// <summary>
		/// スポットライト
		/// </summary>
		struct SpotLight {
			Vector3 position;  // 位置
			float padding;	   // パディング
			Vector3 direction; // 向き
			float intensity;   // 輝度
			float distance;	   // 光の届く最大距離
			float decay;	   // 減衰率
			float cosAngle;	   // 角度
			float cosFalloffStart; // 減衰開始角度
			Vector4 color;	   // 色
		};

		/// <summary>
		/// 環境マップ
		/// </summary>
		struct EnvironmentMap {
			float intensity; // 輝度
		};

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// ImGui表示
		/// </summary>
		void ShowImGui();

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
	private:
		
		/// <summary>
		/// 平行光源の作成
		/// </summary>
		void CreateDirectionalLight();

		/// <summary>
		/// 点光源の作成
		/// </summary>
		void CreatePointLight();

		/// <summary>
		/// スポットライトの作成
		/// </summary>
		void CreateSpotLight();

		/// <summary>
		/// 環境マップの作成
		/// </summary>
		void CreateEnvironmentMap();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
	private:

		/// ========== 平行光源 ========== ///

		// 平行光源リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;

		// 平行光源データ
		DirectionalLight* directionalLightData_ = nullptr;

		/// ========== 点光源 ========== ///

		// 点光源リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource_ = nullptr;

		// 点光源データ
		PointLight* pointLightData_ = nullptr;

		/// ========== スポットライト ========== ///

		// スポットライトリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource_ = nullptr;

		// スポットライトデータ
		SpotLight* spotLightData_ = nullptr;

		/// ========== 環境マップ ========== ///

		// 環境マップリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> environmentMapResource_ = nullptr;

		// 環境マップデータ
		EnvironmentMap* environmentMapData_ = nullptr;

		/// ========== 借りポインタ ========== ///

		// DirectXUtilityのインスタンス
		DirectXUtility* dxUtility_ = nullptr;
	};
}


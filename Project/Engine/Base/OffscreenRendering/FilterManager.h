#pragma once

#include "Filters/BaseFilter.h"
#include "Filters/FullScreenFilter.h"
#include "Filters/GrayscaleFilter.h"
#include "Filters/VignetteFilter.h"
#include "Filters/BoxBlurFilter.h"
#include "Filters/GaussianBlurFilter.h"
#include "Filters/LuminanceOutlineFilter.h"
#include "Filters/DepthOutlineFilter.h"
#include "Filters/RadialBlurFilter.h"
#include "Filters/DissolveFilter.h"
#include "Filters/RandomFilter.h"

#include <unordered_map>
#include <vector>
#include <memory>

/// ===== 前方宣言 ===== ///
class PostEffect;
class Camera;

/// ===== フィルター管理クラス ===== ///
class FilterManager {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static FilterManager* instance;

	// コンストラクタの隠蔽
	FilterManager() = default;
	// デストラクタの隠蔽
	~FilterManager() = default;
	// コピーコンストラクタの封印
	FilterManager(FilterManager&) = delete;
	// コピー代入演算子の封印
	FilterManager& operator=(const FilterManager&) = delete;
	
///-------------------------------------------///
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static FilterManager* GetInstance();

	/// <summary>
	/// カメラのゲッター
	/// </summary>
	/// <returns></returns>
	Camera* GetCamera() const { return camera; }

	/// <summary>
	/// ビネットフィルターのゲッター
	/// </summary>
	/// <returns></returns>
	VignetteFilter* GetVignetteFilter() const { return static_cast<VignetteFilter*>(filters_.at("Vignette").get()); }

	/// <summary>
	/// ラジアルブラーのゲッター
	/// </summary>
	/// <returns></returns>
	RadialBlurFilter* GetRadialBlurFilter() const { return static_cast<RadialBlurFilter*>(filters_.at("RadialBlur").get()); }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:
	
	/// <summary>
	/// PostEffectのセッター
	/// </summary>
	/// <param name="postEffect"></param>
	void SetPostEffect(PostEffect* postEffect) { this->postEffect = postEffect; }

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { this->camera = camera; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// フィルターのリスト
	std::unordered_map<std::string, std::unique_ptr<BaseFilter>> filters_ = {};

	// フィルターの名前の配列
	std::vector<std::string> filterOrder;

	// フルスクリーンフィルター
	std::unique_ptr<FullScreenFilter> fullScreenFilter_ = nullptr;

	// グレースケールフィルター
	std::unique_ptr<GrayscaleFilter> grayscaleFilter_ = nullptr;

	// ビネットフィルター
	std::unique_ptr<VignetteFilter> vignetteFilter_ = nullptr;

	// ボックスブラーフィルター
	std::unique_ptr<BoxBlurFilter> boxBlurFilter_ = nullptr;

	// ガウシアンブラーフィルター
	std::unique_ptr<GaussianBlurFilter> gaussianBlurFilter_ = nullptr;

	// 輝度アウトラインフィルター
	std::unique_ptr<LuminanceOutlineFilter> luminanceOutlineFilter_ = nullptr;

	// 深度アウトラインフィルター
	std::unique_ptr<DepthOutlineFilter> depthOutlineFilter_ = nullptr;

	// ラジアルブラー
	std::unique_ptr<RadialBlurFilter> radialBlurFilter_ = nullptr;

	// ディゾルブフィルター
	std::unique_ptr<DissolveFilter> dissolveFilter_ = nullptr;

	// ランダムフィルター
	std::unique_ptr<RandomFilter> randomFilter_ = nullptr;

	// PostEffectの借りポインタ
	PostEffect* postEffect = nullptr;

	// カメラの借りポインタ
	Camera* camera = nullptr;
};


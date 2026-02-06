#pragma once

#include "WorldTransform.h"

#include <string>

namespace Engine {

	// エミッターの発射タイプ
	enum class EmitterType {
		Constant,   // 常に発射
		Interval,   // 一定間隔ごとに発射
		OneShot,    // 呼んだときだけ
	};

	/// ===== パーティクルエミッター ===== ///
	class ParticleEmitter {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="effectName">エフェクト名</param>
		/// <param name="type">発射タイプ</param>
		/// <param name="count">1回あたりの発生数</param>
		ParticleEmitter(std::string	effectName, EmitterType type, uint32_t count) :
			effectName(effectName),
			emitterType(type),
			count(count) {
		};

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// パーティクル発生
		/// </summary>
		void Emit();

		///-------------------------------------------/// 
		/// セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// 位置のセッター
		/// </summary>
		/// <param name="translate">位置</param>
		void SetTranslate(const Vector3& translate) { worldTransform.SetTranslate(translate); }

		/// <summary>
		/// 頻度のセッター
		/// </summary>
		/// <param name="frequency"></param>
		void SetFrequency(float frequency) { this->frequency = frequency; }

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		// エフェクト名
		std::string effectName;

		// 発射タイプ
		EmitterType emitterType;

		// 発生頻度 (秒間)
		float frequency = 0.0f;

		// 発生用タイマー
		float timer = 0.0f;

		// デルタタイム
		const float deltaTime = 1.0f / 60.0f;

		// 1回あたりの発生数
		uint32_t count = 0;

		// ワールド変換情報
		WorldTransform worldTransform;
	};
}

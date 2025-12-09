#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "MathVector.h"

using namespace MathVector;
using namespace MathRandom;

void ParticleEmitter::Initialize() {

	// ワールド変換の初期化
	worldTransform.Initialize();
}

void ParticleEmitter::Update() {

	//// タイマー更新
	//timer += deltaTime;
	//
	//// 発生頻度を超えたらパーティクル発生
	//if (timer >= frequency) {
	//	Emit();
	//	timer = 0.0f;
	//}

	// ワールド変換の更新
	worldTransform.UpdateMatrix();
}

void ParticleEmitter::Emit() {
	
	// マネージャーからパーティクル設定を取得
	ParticleSetting* setting = ParticleManager::GetInstance()->GetSetting(effectName);

	// 設定がなければ抜ける
	if (!setting) return;

	// パーティクルインスタンスを作成
	for (uint32_t i = 0; i < count; ++i) {

		// 新しいインスタンスを作成
		ParticleInstance particle;

		// 参照元を紐づけ
		particle.setting = setting;

		// スケールの設定
		if(setting->scaleRandom) {

			// 範囲からランダム生成
			particle.scale = RandomVector3(setting->scaleRange);
		}
		else {

			// 固定値を代入
			particle.scale = setting->scale;
		}

		// 回転の設定
		if (setting->rotateRandom) {

			// 範囲からランダム生成
			particle.rotate = RandomVector3(setting->rotateRange);
		}
		else {

			// 固定値を代入
			particle.rotate = setting->rotate;
		}

		// 位置の設定
		if (setting->translateRandom) {

			// 範囲からランダム生成
			particle.translate = RandomVector3(setting->translateRange) + worldTransform.GetTranslate();
		}
		else {

			// 固定値を代入
			particle.translate = setting->translate + worldTransform.GetTranslate();
		}

		// 速度の設定
		if (setting->velocityRandom) {

			// 範囲からランダム生成
			particle.velocity = RandomVector3(setting->velocityRange);
		}
		else {

			// 固定値を代入
			particle.velocity = setting->velocity;
		}

		// 加速度の設定
		if (setting->accelerationRandom) {

			// 範囲からランダム生成
			particle.acceleration = RandomVector3(setting->accelerationRange);
		}
		else {

			// 固定値を代入
			particle.acceleration = setting->acceleration;
		}

		// 色の設定
		if (setting->colorRandom) {

			// 範囲からランダム生成
			particle.color = RandomVector4(setting->colorRange);
		}
		else {

			// 固定値を代入
			particle.color = setting->color;
		}

		// 寿命の設定
		if (setting->lifeTimeRandom) {

			// 範囲からランダム生成
			particle.lifeTime = RandomFloat(setting->lifeTimeRange);
		}
		else {

			// 固定値を代入
			particle.lifeTime = setting->lifeTime;
		}

		// マネージャーにインスタンスを追加
		ParticleManager::GetInstance()->AddInstance(particle);
	}
}
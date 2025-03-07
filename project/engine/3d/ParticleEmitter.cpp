#include "ParticleEmitter.h"
#include "ParticleSystem.h"

ParticleEmitter::ParticleEmitter(const std::string& name, const Transform& transform, uint32_t count, float frequency) :

	// 名前
	name(name),
	// 位置
	transform(transform),
	// 発生数
	count(count),
	// 発生頻度
	frequency(frequency) {
}

void ParticleEmitter::Emit() {

	// 時間を進める
	frequencyTime += kDeltaTime;

	// 頻度より大きいなら発生
	if (frequency <= frequencyTime) {

		// 発生処理
		ParticleSystem::GetInstance()->Emit(name, transform.translate, count);

		// 余計に過ぎた時間も加味して頻度計算する
		frequencyTime -= frequency;
	}
}
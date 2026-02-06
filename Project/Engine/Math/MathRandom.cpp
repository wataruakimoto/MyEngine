#include "MathRandom.h"

#include <random>

using namespace Engine;

// 変数は外部からアクセスできないようにする
namespace {
    std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
}

float MathRandom::RandomFloat(float min, float max) {
    
	// 指定された範囲でランダムなfloatを生成
    std::uniform_real_distribution<float> distribution(min, max);
	// 分布に基づいてランダムな値を生成して返す
	return distribution(randomEngine);
}

float MathRandom::RandomFloat(const floatRange& range) {
   
	return RandomFloat(range.min, range.max);
}

Vector2 MathRandom::RandomVector2(const Vector2Range& range) {
  
	Vector2 result;
	result.x = RandomFloat(range.min.x, range.max.x);
	result.y = RandomFloat(range.min.y, range.max.y);
	return result;
}

Vector3 MathRandom::RandomVector3(const Vector3Range& range) {
    
	Vector3 result;
	result.x = RandomFloat(range.min.x, range.max.x);
	result.y = RandomFloat(range.min.y, range.max.y);
	result.z = RandomFloat(range.min.z, range.max.z);
	return result;
}

Vector4 MathRandom::RandomVector4(const Vector4Range& range) {
    
	Vector4 result;
	result.x = RandomFloat(range.min.x, range.max.x);
	result.y = RandomFloat(range.min.y, range.max.y);
	result.z = RandomFloat(range.min.z, range.max.z);
	result.w = RandomFloat(range.min.w, range.max.w);
	return result;
}

#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "MathRandom.h"

#include <string>
#include <list>
#include <d3d12.h>
#include <wrl.h>
#include <json.hpp>

namespace Engine {

	// パーティクルの形状
	enum class ParticleShape {
		PLANE,
		RING,
		CYLINDER,
		CUBE,
		SPHERE,
		SHARD,
	};

	// マテリアルデータ
	struct Material {
		Vector4 color;
		Matrix4x4 uvTransform;
	};

	// シェーダー用パーティクルデータ
	struct InstanceData {
		Matrix4x4 world; // ワールド行列
		Matrix4x4 WVP; // ワールドビュー射影行列
		Vector4 color; // 色
	};

	// どんなパーティクルを作るかの設定(Blueprint)
	struct ParticleSetting {

		std::string effectName = ""; // エフェクト名
		std::string textureFileName = ""; // テクスチャファイル名
		std::string textureFullPath = ""; // テクスチャのフルパス
		ParticleShape shape = ParticleShape::PLANE; // パーティクルの形状

		bool useBillboard = true; // ビルボードを使うかどうか
		bool useGravity = false; // 重力を使うかどうか

		bool scaleRandom = false; // スケールをランダムにするかどうか
		Vector3Range scaleRange = {}; // スケールの範囲
		Vector3 scale = { 1.0f,1.0f,1.0f }; // スケールの固定値

		bool rotateRandom = false; // 回転をランダムにするかどうか
		Vector3Range rotateRange = {}; // 回転の範囲
		Vector3 rotate = {}; // 回転の固定値

		bool translateRandom = false; // 位置をランダムにするかどうか
		Vector3Range translateRange = {}; // 位置の範囲
		Vector3 translate = {}; // 位置の固定値

		bool velocityRandom = false; // 速度をランダムにするかどうか
		Vector3Range velocityRange = {}; // スケールの範囲
		Vector3 velocity = {}; // 速度の固定値

		bool accelerationRandom = false; // 加速度をランダムにするかどうか
		Vector3Range accelerationRange = {}; // 加速度の範囲
		Vector3 acceleration = {}; // 加速度の固定値

		bool colorRandom = false; // 色をランダムにするかどうか
		Vector4Range colorRange = {}; // 色の範囲
		Vector4 color = { 1.0f,1.0f,1.0f,1.0f }; // 色の固定値

		bool lifeTimeRandom = false; // 寿命をランダムにするかどうか
		floatRange lifeTimeRange = {}; // 寿命の範囲
		float lifeTime = 1.0f; // 寿命の固定値
	};

	// パーティクルのインスタンスデータ(Flyweight)
	struct ParticleInstance {

		// Flyweightパターンの参照元
		const ParticleSetting* setting; // 参照元の設定

		Vector3 scale = {}; // スケール
		Vector3 rotate = {}; // 回転
		Vector3 translate = {}; // 位置
		Vector3 velocity = {}; // 速度
		Vector3 acceleration = {}; // 加速度
		Vector4 color = {}; // 色
		float lifeTime = 0.0f; // 寿命
		float currentTime = 0.0f; // 現在の時間
	};

	// パーティクルのグループデータ
	struct ParticleGroup {

		std::list<ParticleInstance> particles; // パーティクルのリスト
		bool isResourceCreated = false; // リソースが作成済みかどうか

		// GPUリソース関連
		Microsoft::WRL::ComPtr <ID3D12Resource> instanceResource; // インスタンスリソース
		InstanceData* instanceData = nullptr; // インスタンスデータ
		uint32_t srvIndex = 0; // SRVのインデックス
		const uint16_t kMaxInstanceCount = 1024; // インスタンスの最大数 2^10
		uint16_t numInstance = 0; // インスタンスの数
	};

	// JSON用のシリアライズ・デシリアライズ定義
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ParticleSetting,
		effectName, textureFileName,
		textureFullPath, shape,
		useBillboard, useGravity,
		scaleRandom, scaleRange, scale,
		rotateRandom, rotateRange, rotate,
		translateRandom, translateRange, translate,
		velocityRandom, velocityRange, velocity,
		accelerationRandom, accelerationRange, acceleration,
		colorRandom, colorRange, color,
		lifeTimeRandom, lifeTimeRange, lifeTime
	)

		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ParticleInstance,
			setting,
			scale, rotate, translate,
			velocity, acceleration,
			color, lifeTime, currentTime
		)

		NLOHMANN_JSON_SERIALIZE_ENUM(ParticleShape,
			{
			{ParticleShape::PLANE, "PLANE"},
			{ParticleShape::RING, "RING"},
			{ParticleShape::CYLINDER, "CYLINDER"},
			{ParticleShape::CUBE, "CUBE"},
			{ParticleShape::SPHERE, "SPHERE"},
			{ParticleShape::SHARD, "SHARD"},
			})
}
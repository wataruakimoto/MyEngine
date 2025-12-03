#pragma once

#include "ParticleRendererBase.h"

class CubeRenderer : public ParticleRendererBase {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	void Initialize() override;

	void Draw(const std::list<ParticleInstance>& particles, const Camera& camera) override;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	void GenerateVertexData() override;

	void GenerateIndexData() override;

	void GenerateMaterialData() override;

	void GenerateInstanceData() override;
};


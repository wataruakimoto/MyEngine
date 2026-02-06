#pragma once

#include "ParticleRendererBase.h"

namespace Engine {

	class ShardRenderer : public ParticleRendererBase {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		void Initialize() override;

		void Update() override;

		void Draw(uint16_t instanceCount, uint16_t instanceSrvIndex, const std::string& texturePath) override;

		///-------------------------------------------/// 
		/// クラス内関数
		///-------------------------------------------///
	private:

		void GenerateVertexData() override;

		void GenerateIndexData() override;

		void GenerateMaterialData() override;
	};
}

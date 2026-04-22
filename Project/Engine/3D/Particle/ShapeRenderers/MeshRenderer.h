#pragma once

#include "ParticleRendererBase.h"
#include "Data/ModelData.h"

namespace Engine {

	/// ==========  前方宣言 ========== ///

	class ModelManager;

	/// <summary>
	/// メッシュレンダラー
	/// </summary>
	class MeshRenderer : public ParticleRendererBase {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;

		/// <summary>
		/// インスタンス描画
		/// </summary>
		/// <param name="instanceCount">インスタンスの数</param>
		/// <param name="instanceSrvIndex">SRV番号</param>
		/// <param name="texturePath">テクスチャのパス</param>
		void Draw(uint16_t instanceCount, uint16_t instanceSrvIndex, const std::string& texturePath) override;

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
	private:

		void GenerateVertexData() override;

		void GenerateIndexData() override;

		void GenerateMaterialData() override;

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
	private:

		// モデルのデータを格納するポインタ
		ModelData* modelData = nullptr;

		// モデルマネージャのインスタンス
		ModelManager* modelManager = nullptr;
	};
}


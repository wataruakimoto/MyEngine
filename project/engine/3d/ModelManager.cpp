#include "ModelManager.h"
#include "Model.h"
#include <fstream>

ModelManager* ModelManager::instance = nullptr;

ModelManager* ModelManager::GetInstance() {

	if (instance == nullptr) {
		instance = new ModelManager;
	}
	return instance;
}

void ModelManager::Initialize(DirectXCommon* dxCommon) {}

void ModelManager::Finalize() {

	delete instance;
	instance = nullptr;
}

void ModelManager::LoadModel(const std::string& filePath) {

	// 読み込み済みモデルを検索
	if (models.contains(filePath)) {

		// 読み込み済みなら早期return
		return;
	}

	// モデルの生成とファイル読み込み、初期化
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Initialize("resources", filePath);

	// モデルをmapコンテナに格納する
	models.insert(std::make_pair(filePath, std::move(model)));
}

Model* ModelManager::FindModel(const std::string& filePath) {

	// 読み込み済みモデルを検索
	if (models.contains(filePath)) {

		// 読み込み済みなら早期return
		return models.at(filePath).get();
	}

	// ファイル名一致なし
	return nullptr;
}

MaterialData ModelManager::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {

	// 1. 中で必要となる変数の宣言
	MaterialData materialData; // 構築するMaterialData
	std::string line; // ファイルから読んだ1行を格納するもの

	// 2. ファイルを開く
	std::ifstream file(directoryPath + "/" + filename); //ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める

	// 3. 実際にファイルを読み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {

			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	// 4. MaterialDataを返す
	return materialData;
}

ModelData ModelManager::LoadObjFile(const std::string& directoryPath, const std::string& filename) {

	// 1. 中で必要となる変数の宣言
	ModelData modelData; // 構築するModelData
	std::vector<Vector4>positions; // 位置
	std::vector<Vector3>normals; // 法線
	std::vector<Vector2>texcoords; // テクスチャ座標
	std::string line; // ファイルから読んだ１行を格納するもの

	// 2. ファイルを開く
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める

	// 3. 実際にファイルを読み、ModelDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む

		// identifierに応じた処理
		if (identifier == "v") {

			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);

		}
		else if (identifier == "vt") {

			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);

		}
		else if (identifier == "vn") {

			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);

		}
		else if (identifier == "f") {

			VertexData triangle[3];

			// 面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/'); // /区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];

				position.x *= -1.0f; // 位置xを反転
				texcoord.y = 1.0f - texcoord.y; // 原点を左下から左上に変更
				normal.x *= -1.0f; // 法線xも反転

				//VertexData vertex = { position,texcoord,normal };
				//modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position,texcoord,normal };
			}

			// 頂点を逆順で登録することで周り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);

		}
		else if (identifier == "mtllib") {

			// materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}

	// 4. ModelDataを返す
	return modelData;
}
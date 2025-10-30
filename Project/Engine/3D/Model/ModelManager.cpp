#include "ModelManager.h"
#include "Logger.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>

using namespace Logger;

ModelManager* ModelManager::instance = nullptr;

ModelManager* ModelManager::GetInstance() {

	if (instance == nullptr) {
		instance = new ModelManager;
	}
	return instance;
}

void ModelManager::Initialize() {}

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

void ModelManager::LoadModelData(const std::string& directoryPath, const std::string& filename) {

	// ファイルパスを作成
	std::string filePath = directoryPath + "/" + filename;

	// ファイルが存在するか確認
	std::ifstream file(filePath);
	assert(file.is_open()); // ファイルが開けなかったら止める

	// 読み込み済みかモデルを検索
	if (modelDatas.contains(filePath)) {

		// 読み込み済みなら早期return
		return;
	}

	// モデルデータを読み込む
	std::unique_ptr<ModelData> modelData = std::make_unique<ModelData>();

	// assimpを使ってファイルを読み込む
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // meshがないのは対応しない

	// meshを解析する
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {

		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないmeshは対応しない
		assert(mesh->HasTextureCoords(0)); // Texcoordがないmeshは対応しない

		// faceを解析する
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {

			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形以外は対応しない

			//vertexを解析する
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {

				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex;
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };

				// 左手座標系に対応させる
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;

				modelData->vertices.push_back(vertex);
			}
		}
	}

	// materialを解析する
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {

		aiMaterial* material = scene->mMaterials[materialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {

			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData->material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}

	modelData->rootNode = ReadNode(scene->mRootNode);

	// モデルデータをmapコンテナに格納する
	modelDatas.insert(std::make_pair(filePath, std::move(modelData)));
}

ModelData* ModelManager::FindModelData(const std::string& directoryPath, const std::string& filename) {

	// ファイルパスを作成
	std::string filePath = directoryPath + "/" + filename;
	
	// 読み込み済みモデルを検索
	if (modelDatas.contains(filePath)) {

		// 読み込み済みなら早期return
		return modelDatas.at(filePath).get();
	}

	// ファイル名一致なしならログを出す
	Log("ModelManager::FindModelData: Model data not found for " + filePath + "\n");
	return nullptr;
}

Node ModelManager::ReadNode(aiNode* node) {

	Node result;

	aiMatrix4x4 aiLocalMatrix = node->mTransformation; // nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose(); // 列ベクトル形式を行ベクトル形式に転置

	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			result.localMatrix.m[i][j] = aiLocalMatrix[j][i];
		}
	}

	result.name = node->mName.C_Str(); // Node名を格納
	result.children.resize(node->mNumChildren); // 子ノードの数だけ確保

	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {

		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}

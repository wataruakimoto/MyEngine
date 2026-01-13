#include "ModelManager.h"
#include "Texture/TextureManager.h"
#include "Logger.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <filesystem>

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

void ModelManager::LoadModelData(const std::string& directoryName, const std::string& fileName) {

	// マップコンテナに登録するためのキーを作成
	std::string key = directoryName + "/" + fileName; // キー

	// 読み込み済みなら早期return
	if (modelDatas.contains(key)) return;

	// モデルのファイルまでのフルパスを作成
	std::string fullPath = baseDirectoryPath + "/" + directoryName + "/" + fileName; // フルパス

	// ファイルが存在するか確認
	std::ifstream file(fullPath);
	assert(file.is_open()); // ファイルが開けなかったら止める

	// assimpを使ってファイルを読み込む
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fullPath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // meshがないのは対応しない

	// モデルデータを作成
	std::unique_ptr<ModelData> modelData = std::make_unique<ModelData>();

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

			// テクスチャファイルパスを取得
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);

			// 文字列に変換
			std::string textureFileName = textureFilePath.C_Str();

			// ファイル名だけを抽出
			std::string filename = std::filesystem::path(textureFileName).filename().string();

			// テクスチャファイルを探索
			std::string foundTextureFilePath = FindTextureFilePath(directoryName, filename);

			// テクスチャ読み込み
			TextureManager::GetInstance()->LoadTexture(foundTextureFilePath);

			// 見つかったテクスチャファイルパスをモデルデータに格納
			modelData->material.textureFilePath = foundTextureFilePath;
		}
	}

	modelData->rootNode = ReadNode(scene->mRootNode);

	// モデルデータをmapコンテナに格納する
	modelDatas.insert(std::make_pair(key, std::move(modelData)));
}

ModelData* ModelManager::FindModelData(const std::string& directoryName, const std::string& fileName) {

	// マップコンテナから検索するためのキーを作成
	std::string key = directoryName + "/" + fileName; // キー
	
	// 読み込み済みモデルを検索
	if (modelDatas.contains(key)) {

		// 一致したらモデルデータを返す
		return modelDatas.at(key).get();
	}

	// キーと一致するモデルデータが見つからなかったらログ出してnullptrを返す
	Log("ModelManager::FindModelData: Model data not found for " + key + "\n");
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

std::string ModelManager::FindTextureFilePath(const std::string& directoryName, const std::string& filename) {

	// TextureManagerのベースディレクトリパスを取得
	std::string textureBaseDirectoryPath = TextureManager::GetInstance()->GetBaseDirectoryPath();

	// パターン１： Textures/ディレクトリ名/ファイル名
	std::string path1 = textureBaseDirectoryPath + "/" + directoryName + "/" + filename;

	// ファイルが存在したらパスを返す
	if (std::filesystem::exists(path1)) return path1;

	// パターン２： Textures/ファイル名
	std::string path2 = textureBaseDirectoryPath + "/" + filename;

	// ファイルが存在したらパスを返す
	if (std::filesystem::exists(path2)) return path2;

	// パターン3 : Models/ディレクトリ名/ファイル名
	std::string path3 = baseDirectoryPath + "/" + directoryName + "/" + filename;

	// ファイルが存在したらパスを返す
	if (std::filesystem::exists(path3)) return path3;

	return textureBaseDirectoryPath + "/" + "White.png"; // 見つからなかったら
}

#include "ModelManager.h"
#include "Texture/TextureManager.h"
#include "Logger.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <filesystem>

using namespace Engine;
using namespace Logger;

void ModelManager::Initialize() {}

void ModelManager::Finalize() {

	delete instance;
	instance = nullptr;
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

void ModelManager::LoadModel(const std::string& relativePath) {

	// モデルのファイルまでのフルパスを作成
	std::string fullPath = baseDirectoryPath + "/" + relativePath; // フルパス

	// フルパスを指定してモデルを読み込む
	LoadModelBase(fullPath);
}

void ModelManager::LoadModelFullPath(const std::string& fullPath) {

	// フルパスを指定してモデルを読み込む
	LoadModelBase(fullPath);
}

ModelData* ModelManager::FindModel(const std::string& fileName) {

	// モデルのファイルまでのフルパスを作成
	std::string fullPath = baseDirectoryPath + "/" + fileName; // フルパス

	// フルパスを指定してモデルデータを検索
	return FindModelBase(fullPath);
}

ModelData* ModelManager::FindModelFullPath(const std::string& fullPath) {
	
	return FindModelBase(fullPath);
}

void ModelManager::LoadModelBase(const std::string& fullPath) {

	// 読み込み済みなら早期return
	if (modelDatas.contains(fullPath)) return;

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
			std::string fileName = std::filesystem::path(textureFileName).filename().string();

			// テクスチャファイルを探索
			std::string foundTextureFilePath = FindTextureFilePath(fullPath, fileName);

			// テクスチャ読み込み
			TextureManager::GetInstance()->LoadTextureFullPath(foundTextureFilePath);

			// 見つかったテクスチャファイルパスをモデルデータに格納
			modelData->material.textureFilePath = foundTextureFilePath;
		}
	}

	modelData->rootNode = ReadNode(scene->mRootNode);

	// モデルデータをmapコンテナに格納する
	modelDatas.insert(std::make_pair(fullPath, std::move(modelData)));
}

std::string ModelManager::FindTextureFilePath(const std::string& modelFullPath, const std::string& filename) {

	// TextureManagerのベースディレクトリパスを取得 (Resources/Textures)
	std::string textureBaseDirectoryPath = TextureManager::GetInstance()->GetBaseDirectoryPath();

	// モデルの親ディレクトリパスを取得 (Resources/Models/ディレクトリ名)
	std::filesystem::path modelDirectoryPath = std::filesystem::path(modelFullPath).parent_path();

	// パターン１： Resources/Textures/ディレクトリ名/ファイル名
	std::filesystem::path relativeDirectory = std::filesystem::relative(modelDirectoryPath, baseDirectoryPath);
	std::filesystem::path path1 = std::filesystem::path(textureBaseDirectoryPath) / relativeDirectory / filename;

	// ファイルが存在したらパスを返す
	if (std::filesystem::exists(path1)) return path1.generic_string();

	// パターン２： Resources/Textures/ファイル名
	std::filesystem::path path2 = std::filesystem::path(textureBaseDirectoryPath) / filename;

	// ファイルが存在したらパスを返す
	if (std::filesystem::exists(path2)) return path2.generic_string();

	// パターン3 : Resources/Models/ディレクトリ名/ファイル名
	std::filesystem::path path3 = modelDirectoryPath / filename;

	// ファイルが存在したらパスを返す
	if (std::filesystem::exists(path3)) return path3.generic_string();

	// どのパターンでも見つからなかったらログ出す
	Log("ModelManager::FindTextureFilePath: Texture file not found for " + filename + "\n");

	// 空文字を返す
	return "";
}

ModelData* ModelManager::FindModelBase(const std::string& fullPath) {
	
	// マップコンテナからフルパスをキーに検索
	if (modelDatas.contains(fullPath)) {

		// 一致したらモデルデータを返す
		return modelDatas.at(fullPath).get();
	}

	// キーと一致するモデルデータが見つからないのでログ出す
	Log("ModelManager::FindModelData: Model data not found for " + fullPath + "\n");

	// nullptrを返す
	return nullptr;
}

ModelManager* ModelManager::instance = nullptr;

ModelManager* ModelManager::GetInstance() {

	if (instance == nullptr) {
		instance = new ModelManager;
	}
	return instance;
}

#pragma once
#include "MaterialData.h"
#include "VertexData.h"
#include <vector>

// ノード
struct Node {
	Matrix4x4 localMatrix;		// ローカル行列
	std::string name;			// ノード名
	std::vector<Node> children; // 子ノード
};

// モデルデータ
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	Node rootNode;
};
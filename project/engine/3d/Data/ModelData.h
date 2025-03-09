#pragma once
#include "MaterialData.h"
#include "VertexData.h"
#include <vector>

// モデルデータ
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};
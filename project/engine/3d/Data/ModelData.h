#pragma once
#include "VertexData.h"
#include "MaterialData.h"
#include <vector>

// モデルデータ
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};
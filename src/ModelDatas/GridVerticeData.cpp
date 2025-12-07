#include"GridVerticeData.h"
ModelDataInfo GridVerticeData::GetModelDatas()
{
	ModelDataInfo info;
	info.vertices_datas = GenerateGridVertices();
	return info;
}

std::vector<float> GridVerticeData::GenerateGridVertices()
{
	std::vector<float> vertices;
	for (int i = 0; i <= gridSize; ++i)
	{
		float x = -5.0f + i;

		x = i * gridSpacing - (gridSize * gridSpacing) / 2;
		vertices.push_back(x);

		vertices.push_back(-0.5f);
		vertices.push_back(-(gridSize * gridSpacing) / 2);
		vertices.push_back(x);

		vertices.push_back(-0.5f);
		vertices.push_back((gridSize * gridSpacing) / 2);
	}
	for (int i = 0; i <= gridSize; ++i) {
		float z = -5.0f + i;
		z = i * gridSpacing - (gridSize * gridSpacing) / 2;
		vertices.push_back(-(gridSize * gridSpacing) / 2);

		vertices.push_back(-0.5f);
		vertices.push_back(z);
		vertices.push_back((gridSize * gridSpacing) / 2);

		vertices.push_back(-0.5f);
		vertices.push_back(z);
	}
	return vertices;
}

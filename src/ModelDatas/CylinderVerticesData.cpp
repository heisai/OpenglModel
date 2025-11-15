#include"CylinderVerticesData.h"
ModelDataInfo CylinderVerticesData::GetModelDatas()
{
    ModelDataInfo info;
    info.vertices_datas = GenerateCylinderVertices(0.8f, 2.5f, 36);
    info.indices_datas = GenerateCylinderIndices(36);
    return info;
}

std::vector<float> CylinderVerticesData::GenerateCylinderVertices(float radius, float height, int sectors) {
	std::vector<float> vertices;

	const float PI = 3.14159265358979323846f;
	float sectorStep = 2 * PI / sectors;

	// 生成侧面顶点
	for (int i = 0; i <= sectors; ++i)
	{
		float sectorAngle = i * sectorStep;
		float cosAngle = cos(sectorAngle);
		float sinAngle = sin(sectorAngle);

		// 底部顶点
		vertices.push_back(radius * cosAngle);      // x
		vertices.push_back(-height / 2.0f);         // y
		vertices.push_back(radius * sinAngle);      // z
		vertices.push_back(cosAngle);               // nx
		vertices.push_back(0.0f);                   // ny
		vertices.push_back(sinAngle);               // nz

		// 顶部顶点
		vertices.push_back(radius * cosAngle);      // x
		vertices.push_back(height / 2.0f);          // y
		vertices.push_back(radius * sinAngle);      // z
		vertices.push_back(cosAngle);               // nx
		vertices.push_back(0.0f);                   // ny
		vertices.push_back(sinAngle);               // nz
	}

	// 生成顶部圆面
	vertices.push_back(0.0f);                       // x
	vertices.push_back(height / 2.0f);              // y
	vertices.push_back(0.0f);                       // z
	vertices.push_back(0.0f);                       // nx
	vertices.push_back(1.0f);                       // ny
	vertices.push_back(0.0f);                       // nz

	for (int i = 0; i <= sectors; ++i) 
	{
		float sectorAngle = i * sectorStep;
		float cosAngle = cos(sectorAngle);
		float sinAngle = sin(sectorAngle);

		vertices.push_back(radius * cosAngle);      // x
		vertices.push_back(height / 2.0f);          // y
		vertices.push_back(radius * sinAngle);      // z
		vertices.push_back(0.0f);                   // nx
		vertices.push_back(1.0f);                   // ny
		vertices.push_back(0.0f);                   // nz
	}

	// 生成底部圆面
	vertices.push_back(0.0f);                       // x
	vertices.push_back(-height / 2.0f);             // y
	vertices.push_back(0.0f);                       // z
	vertices.push_back(0.0f);                       // nx
	vertices.push_back(-1.0f);                      // ny
	vertices.push_back(0.0f);                       // nz

	for (int i = 0; i <= sectors; ++i) 
	{
		float sectorAngle = i * sectorStep;
		float cosAngle = cos(sectorAngle);
		float sinAngle = sin(sectorAngle);

		vertices.push_back(radius * cosAngle);      // x
		vertices.push_back(-height / 2.0f);         // y
		vertices.push_back(radius * sinAngle);      // z
		vertices.push_back(0.0f);                   // nx
		vertices.push_back(-1.0f);                  // ny
		vertices.push_back(0.0f);                   // nz
	}

	return vertices;
}

std::vector<unsigned int> CylinderVerticesData::GenerateCylinderIndices(int sectors)
{
	std::vector<unsigned int> indices;

	// 生成侧面索引
	for (int i = 0; i < sectors; ++i) 
	{
		int base = i * 2;
		indices.push_back(base);
		indices.push_back(base + 1);
		indices.push_back(base + 2);

		indices.push_back(base + 1);
		indices.push_back(base + 3);
		indices.push_back(base + 2);
	}

	// 生成顶部圆面索引
	int topCenter = (sectors + 1) * 2; // 侧面顶点数量
	for (int i = 0; i < sectors; ++i) 
	{
		indices.push_back(topCenter);
		indices.push_back(topCenter + i + 1);
		indices.push_back(topCenter + i + 2);
	}

	// 生成底部圆面索引
	int bottomCenter = topCenter + sectors + 2; // 侧面 + 顶部顶点数量
	for (int i = 0; i < sectors; ++i) 
	{
		indices.push_back(bottomCenter);
		indices.push_back(bottomCenter + i + 2);
		indices.push_back(bottomCenter + i + 1);
	}

	return indices;
}



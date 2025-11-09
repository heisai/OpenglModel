#include"ToursVerticesData.h"

ModelDataInfo ToursVerticesData::GetModelDatas()
{
    ModelDataInfo info;
    info.m_Vertices = GenerateTorusVertices(1.0f, 0.4f, 32, 16);
    info.m_Indices = GenerateTorusIndices(32, 16);
    return info;
}

// 简化的顶点数据生成（位置和法线交错存储）
std::vector<float> ToursVerticesData::GenerateTorusVertices(float majorRadius ,
	float minorRadius ,
	unsigned int majorSegments ,
	unsigned int minorSegments ) {
	std::vector<float> vertices;

	for (unsigned int i = 0; i <= majorSegments; ++i)
	{
		float majorAngle = 2.0f * 3.14159265358979323846f * i / majorSegments;
		float cosMajor = cos(majorAngle);
		float sinMajor = sin(majorAngle);

		for (unsigned int j = 0; j <= minorSegments; ++j) {
			float minorAngle = 2.0f * 3.14159265358979323846f * j / minorSegments;
			float cosMinor = cos(minorAngle);
			float sinMinor = sin(minorAngle);

			// 顶点位置
			vertices.push_back((majorRadius + minorRadius * cosMinor) * cosMajor);
			vertices.push_back(minorRadius * sinMinor);
			vertices.push_back((majorRadius + minorRadius * cosMinor) * sinMajor);

			// 法线
			vertices.push_back(cosMinor * cosMajor);
			vertices.push_back(sinMinor);
			vertices.push_back(cosMinor * sinMajor);
		}
	}

	return vertices;
}

// 生成索引数据
std::vector<unsigned int> ToursVerticesData::GenerateTorusIndices(unsigned int majorSegments ,
	unsigned int minorSegments ) 
{
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < majorSegments; ++i)
	{
		for (unsigned int j = 0; j < minorSegments; ++j)
		{
			unsigned int first = i * (minorSegments + 1) + j;
			unsigned int second = (i + 1) * (minorSegments + 1) + j;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	return indices;
}


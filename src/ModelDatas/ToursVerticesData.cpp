#include "ToursVerticesData.h"
#include <cmath>

MeshData ToursVerticesData::GetModelDatas()
{
	MeshData data;
	constexpr unsigned int majorSegments = 64;
	constexpr unsigned int minorSegments = 16;
	data.vertices = GenerateTorusVertices(1.0f, 0.4f, majorSegments, minorSegments);
	data.indices = GenerateTorusIndices(majorSegments, minorSegments);
	data.model_name_ = TR("ring").toStdString();
	return data;
}

std::vector<Vertex> ToursVerticesData::GenerateTorusVertices(float majorRadius,
	float minorRadius,
	unsigned int majorSegments,
	unsigned int minorSegments)
{
	std::vector<Vertex> vertices;
	if (majorSegments == 0 || minorSegments == 0)
	{
		return vertices;
	}

	vertices.reserve(static_cast<std::size_t>(majorSegments + 1U) * (minorSegments + 1U));
	constexpr float pi = 3.14159265358979323846f;

	for (unsigned int i = 0; i <= majorSegments; ++i)
	{
		const float u = static_cast<float>(i) / static_cast<float>(majorSegments);
		const float majorAngle = 2.0f * pi * u;
		const float cosMajor = std::cos(majorAngle);
		const float sinMajor = std::sin(majorAngle);

		for (unsigned int j = 0; j <= minorSegments; ++j)
		{
			const float v = static_cast<float>(j) / static_cast<float>(minorSegments);
			const float minorAngle = 2.0f * pi * v;
			const float cosMinor = std::cos(minorAngle);
			const float sinMinor = std::sin(minorAngle);

			Vertex vertex{};
			vertex.position = glm::vec3(
				(majorRadius + minorRadius * cosMinor) * cosMajor,
				minorRadius * sinMinor,
				(majorRadius + minorRadius * cosMinor) * sinMajor);
			vertex.normal = glm::vec3(cosMinor * cosMajor, sinMinor, cosMinor * sinMajor);
			vertex.texCoord = glm::vec2(u, v);
			vertices.push_back(vertex);
		}
	}

	return vertices;
}

std::vector<uint32_t> ToursVerticesData::GenerateTorusIndices(unsigned int majorSegments,
	unsigned int minorSegments)
{
	std::vector<uint32_t> indices;
	indices.reserve(static_cast<std::size_t>(majorSegments) * minorSegments * 6U);

	for (uint32_t i = 0; i < majorSegments; ++i)
	{
		for (uint32_t j = 0; j < minorSegments; ++j)
		{
			const uint32_t first = i * (minorSegments + 1U) + j;
			const uint32_t second = (i + 1U) * (minorSegments + 1U) + j;
			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1U);
			indices.push_back(second);
			indices.push_back(second + 1U);
			indices.push_back(first + 1U);
		}
	}

	return indices;
}


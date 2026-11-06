#include "CylinderVerticesData.h"
#include <cmath>

MeshData CylinderVerticesData::GetModelDatas()
{
	MeshData data;
	data.vertices = GenerateCylinderVertices(0.8f, 2.5f, 36 * 3);
	data.indices = GenerateCylinderIndices(36 * 3);
	data.model_name_ = TR("cylinder").toStdString();
	return data;
}

std::vector<Vertex> CylinderVerticesData::GenerateCylinderVertices(float radius, float height, int sectors)
{
	std::vector<Vertex> vertices;
	if (sectors <= 0)
	{
		return vertices;
	}

	vertices.reserve(static_cast<std::size_t>(sectors) * 4U + 6U);
	constexpr float pi = 3.14159265358979323846f;
	const float sectorStep = 2.0f * pi / static_cast<float>(sectors);

	for (int i = 0; i <= sectors; ++i)
	{
		const float u = static_cast<float>(i) / static_cast<float>(sectors);
		const float sectorAngle = static_cast<float>(i) * sectorStep;
		const float cosAngle = std::cos(sectorAngle);
		const float sinAngle = std::sin(sectorAngle);

		Vertex bottomVertex{};
		bottomVertex.position = glm::vec3(radius * cosAngle, -height / 2.0f, radius * sinAngle);
		bottomVertex.normal = glm::vec3(cosAngle, 0.0f, sinAngle);
		bottomVertex.texCoord = glm::vec2(u, 0.0f);
		vertices.push_back(bottomVertex);

		Vertex topVertex{};
		topVertex.position = glm::vec3(radius * cosAngle, height / 2.0f, radius * sinAngle);
		topVertex.normal = glm::vec3(cosAngle, 0.0f, sinAngle);
		topVertex.texCoord = glm::vec2(u, 1.0f);
		vertices.push_back(topVertex);
	}

	Vertex topCenter{};
	topCenter.position = glm::vec3(0.0f, height / 2.0f, 0.0f);
	topCenter.normal = glm::vec3(0.0f, 1.0f, 0.0f);
	topCenter.texCoord = glm::vec2(0.5f, 0.5f);
	vertices.push_back(topCenter);

	for (int i = 0; i <= sectors; ++i)
	{
		const float sectorAngle = static_cast<float>(i) * sectorStep;
		const float cosAngle = std::cos(sectorAngle);
		const float sinAngle = std::sin(sectorAngle);

		Vertex topRim{};
		topRim.position = glm::vec3(radius * cosAngle, height / 2.0f, radius * sinAngle);
		topRim.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		topRim.texCoord = glm::vec2(0.5f + 0.5f * cosAngle, 0.5f + 0.5f * sinAngle);
		vertices.push_back(topRim);
	}

	Vertex bottomCenter{};
	bottomCenter.position = glm::vec3(0.0f, -height / 2.0f, 0.0f);
	bottomCenter.normal = glm::vec3(0.0f, -1.0f, 0.0f);
	bottomCenter.texCoord = glm::vec2(0.5f, 0.5f);
	vertices.push_back(bottomCenter);

	for (int i = 0; i <= sectors; ++i)
	{
		const float sectorAngle = static_cast<float>(i) * sectorStep;
		const float cosAngle = std::cos(sectorAngle);
		const float sinAngle = std::sin(sectorAngle);

		Vertex bottomRim{};
		bottomRim.position = glm::vec3(radius * cosAngle, -height / 2.0f, radius * sinAngle);
		bottomRim.normal = glm::vec3(0.0f, -1.0f, 0.0f);
		bottomRim.texCoord = glm::vec2(0.5f + 0.5f * cosAngle, 0.5f + 0.5f * sinAngle);
		vertices.push_back(bottomRim);
	}

	return vertices;
}

std::vector<uint32_t> CylinderVerticesData::GenerateCylinderIndices(int sectors)
{
	std::vector<uint32_t> indices;
	if (sectors <= 0)
	{
		return indices;
	}

	const uint32_t sectorCount = static_cast<uint32_t>(sectors);
	indices.reserve(static_cast<std::size_t>(sectorCount) * 12U);

	for (uint32_t i = 0; i < sectorCount; ++i)
	{
		const uint32_t base = i * 2U;
		indices.push_back(base);
		indices.push_back(base + 1U);
		indices.push_back(base + 2U);
		indices.push_back(base + 1U);
		indices.push_back(base + 3U);
		indices.push_back(base + 2U);
	}

	const uint32_t topCenter = (sectorCount + 1U) * 2U;
	for (uint32_t i = 0; i < sectorCount; ++i)
	{
		indices.push_back(topCenter);
		indices.push_back(topCenter + i + 1U);
		indices.push_back(topCenter + i + 2U);
	}

	const uint32_t bottomCenter = topCenter + sectorCount + 2U;
	for (uint32_t i = 0; i < sectorCount; ++i)
	{
		indices.push_back(bottomCenter);
		indices.push_back(bottomCenter + i + 2U);
		indices.push_back(bottomCenter + i + 1U);
	}

	return indices;
}



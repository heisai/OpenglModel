#include "CubeVerticesData.h"

MeshData CubeVerticesData::GetModelDatas()
{
	MeshData data;
	data.vertices = GenerateCubeVertices(1.0f);
	data.indices = GenerateCubeIndices();
	data.model_name_ = TR("cube").toStdString();
	return data;
}

std::vector<Vertex> CubeVerticesData::GenerateCubeVertices(float size)
{
	std::vector<Vertex> vertices;
	vertices.reserve(24);
	const float halfSize = size * 0.75f;

	const glm::vec3 frontPos[4] = {
		glm::vec3(-halfSize, -halfSize, halfSize),
		glm::vec3(halfSize, -halfSize, halfSize),
		glm::vec3(halfSize, halfSize, halfSize),
		glm::vec3(-halfSize, halfSize, halfSize)
	};
	AddFaceVertices(vertices, frontPos, glm::vec3(0.0f, 0.0f, 1.0f));

	const glm::vec3 backPos[4] = {
		glm::vec3(halfSize, -halfSize, -halfSize),
		glm::vec3(-halfSize, -halfSize, -halfSize),
		glm::vec3(-halfSize, halfSize, -halfSize),
		glm::vec3(halfSize, halfSize, -halfSize)
	};
	AddFaceVertices(vertices, backPos, glm::vec3(0.0f, 0.0f, -1.0f));

	const glm::vec3 rightPos[4] = {
		glm::vec3(halfSize, -halfSize, halfSize),
		glm::vec3(halfSize, -halfSize, -halfSize),
		glm::vec3(halfSize, halfSize, -halfSize),
		glm::vec3(halfSize, halfSize, halfSize)
	};
	AddFaceVertices(vertices, rightPos, glm::vec3(1.0f, 0.0f, 0.0f));

	const glm::vec3 leftPos[4] = {
		glm::vec3(-halfSize, -halfSize, -halfSize),
		glm::vec3(-halfSize, -halfSize, halfSize),
		glm::vec3(-halfSize, halfSize, halfSize),
		glm::vec3(-halfSize, halfSize, -halfSize)
	};
	AddFaceVertices(vertices, leftPos, glm::vec3(-1.0f, 0.0f, 0.0f));

	const glm::vec3 topPos[4] = {
		glm::vec3(-halfSize, halfSize, halfSize),
		glm::vec3(halfSize, halfSize, halfSize),
		glm::vec3(halfSize, halfSize, -halfSize),
		glm::vec3(-halfSize, halfSize, -halfSize)
	};
	AddFaceVertices(vertices, topPos, glm::vec3(0.0f, 1.0f, 0.0f));

	const glm::vec3 bottomPos[4] = {
		glm::vec3(-halfSize, -halfSize, -halfSize),
		glm::vec3(halfSize, -halfSize, -halfSize),
		glm::vec3(halfSize, -halfSize, halfSize),
		glm::vec3(-halfSize, -halfSize, halfSize)
	};
	AddFaceVertices(vertices, bottomPos, glm::vec3(0.0f, -1.0f, 0.0f));

	return vertices;
}

std::vector<uint32_t> CubeVerticesData::GenerateCubeIndices()
{
	std::vector<uint32_t> indices;
	indices.reserve(36);

	for (uint32_t face = 0; face < 6; ++face)
	{
		const uint32_t baseVertex = face * 4;
		indices.push_back(baseVertex);
		indices.push_back(baseVertex + 1);
		indices.push_back(baseVertex + 2);
		indices.push_back(baseVertex);
		indices.push_back(baseVertex + 2);
		indices.push_back(baseVertex + 3);
	}

	return indices;
}

void CubeVerticesData::AddFaceVertices(std::vector<Vertex>& vertices,
	const glm::vec3 positions[4],
	const glm::vec3& normal)
{
	const glm::vec2 texCoords[4] = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
	};

	for (std::size_t i = 0; i < 4; ++i)
	{
		Vertex vertex{};
		vertex.position = positions[i];
		vertex.normal = normal;
		vertex.texCoord = texCoords[i];
		vertices.push_back(vertex);
	}
}

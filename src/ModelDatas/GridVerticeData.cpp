#include "GridVerticeData.h"

MeshData GridVerticeData::GetModelDatas()
{
	MeshData data;
	data.vertices = GenerateGridVertices();
	return data;
}

std::vector<Vertex> GridVerticeData::GenerateGridVertices()
{
	const glm::vec3 normal(0.0f, 1.0f, 0.0f);
	const auto makeVertex = [&normal](const glm::vec3& position, const glm::vec2& texCoord)
	{
		Vertex vertex{};
		vertex.position = position;
		vertex.normal = normal;
		vertex.texCoord = texCoord;
		return vertex;
	};

	return {
		makeVertex(glm::vec3(10.0f, -0.5f, 10.0f), glm::vec2(10.0f, 0.0f)),
		makeVertex(glm::vec3(-10.0f, -0.5f, 10.0f), glm::vec2(0.0f, 0.0f)),
		makeVertex(glm::vec3(-10.0f, -0.5f, -10.0f), glm::vec2(0.0f, 10.0f)),
		makeVertex(glm::vec3(10.0f, -0.5f, 10.0f), glm::vec2(10.0f, 0.0f)),
		makeVertex(glm::vec3(-10.0f, -0.5f, -10.0f), glm::vec2(0.0f, 10.0f)),
		makeVertex(glm::vec3(10.0f, -0.5f, -10.0f), glm::vec2(10.0f, 10.0f))
	};
}

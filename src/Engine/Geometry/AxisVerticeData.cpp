#include "AxisVerticeData.h"

MeshData AxisVerticeData::GetModelDatas()
{
	MeshData data;
	data.topology = PrimitiveTopology::Points;
	data.vertices = GenerateAxisVertices();
	return data;
}

std::vector<Vertex> AxisVerticeData::GenerateAxisVertices()
{
	Vertex origin{};
	origin.position = glm::vec3(0.0f, 0.0f, 0.0f);
	return { origin };
}

#include "LightVerticesData.h"

MeshData LightVerticesData::GetModelDatas()
{
	MeshData data;
	const glm::vec3 positions[] = {
		{ -0.5f, -0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{  0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f }
	};

	data.vertices.reserve(8);
	for (const auto& position : positions)
	{
		Vertex vertex{};
		vertex.position = position;
		vertex.color = glm::vec4(1.0f);
		data.vertices.push_back(vertex);
	}

	data.indices = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		1, 4, 7, 1, 7, 2,
		5, 0, 3, 5, 3, 6,
		3, 2, 7, 3, 7, 6,
		5, 4, 1, 5, 1, 0
	};
	data.model_name_ = TR("light").toStdString();
	return data;
}

#include"AxisVerticeData.h"
ModelDataInfo AxisVerticeData::GetModelDatas()
{
	ModelDataInfo info;
	info.vertices_datas = GenerateAxisVertices();
	return info;
}

std::vector<float> AxisVerticeData::GenerateAxisVertices()
{
	std::vector<float> vertices = { 0.0f, 0.0f, 0.0f };// 原点;
	return vertices;
}

#include"CubeVerticesData.h"

ModelDataInfo CubeVerticesData::GetModelDatas()
{
	ModelDataInfo info;
	info.vertices_datas = GenerateCubeVertices(1.0f);
	info.indices_datas = GenerateCubeIndices();
	return info;
}
// 简化的顶点数据生成（位置和法线交错存储）
std::vector<float> CubeVerticesData::GenerateCubeVertices(float size)
{
	std::vector<float> vertices;
	float halfSize = size * 0.75f;

	// 定义6个面的法向量
	const float normals[6][3] = {
		{ 0.0f,  0.0f,  1.0f}, // 前面 (Z+)
		{ 0.0f,  0.0f, -1.0f}, // 后面 (Z-)
		{ 1.0f,  0.0f,  0.0f}, // 右面 (X+)
		{-1.0f,  0.0f,  0.0f}, // 左面 (X-)
		{ 0.0f,  1.0f,  0.0f}, // 上面 (Y+)
		{ 0.0f, -1.0f,  0.0f}  // 下面 (Y-)
	};

	// 前面 (Z+)
	const float frontPos[4][3] = {
		{-halfSize, -halfSize,  halfSize}, // 左下
		{ halfSize, -halfSize,  halfSize}, // 右下
		{ halfSize,  halfSize,  halfSize}, // 右上
		{-halfSize,  halfSize,  halfSize}  // 左上
	};
	AddFaceVertices(vertices, frontPos, normals[0]);

	// 后面 (Z-)
	const float backPos[4][3] = {
		{ halfSize, -halfSize, -halfSize}, // 右下
		{-halfSize, -halfSize, -halfSize}, // 左下
		{-halfSize,  halfSize, -halfSize}, // 左上
		{ halfSize,  halfSize, -halfSize}  // 右上
	};
	AddFaceVertices(vertices, backPos, normals[1]);

	// 右面 (X+)
	const float rightPos[4][3] = {
		{ halfSize, -halfSize,  halfSize}, // 前下
		{ halfSize, -halfSize, -halfSize}, // 后下
		{ halfSize,  halfSize, -halfSize}, // 后上
		{ halfSize,  halfSize,  halfSize}  // 前上
	};
	AddFaceVertices(vertices, rightPos, normals[2]);

	// 左面 (X-)
	const float leftPos[4][3] = {
		{-halfSize, -halfSize, -halfSize}, // 后下
		{-halfSize, -halfSize,  halfSize}, // 前下
		{-halfSize,  halfSize,  halfSize}, // 前上
		{-halfSize,  halfSize, -halfSize}  // 后上
	};
	AddFaceVertices(vertices, leftPos, normals[3]);

	// 上面 (Y+)
	const float topPos[4][3] = {
		{-halfSize,  halfSize,  halfSize}, // 前左
		{ halfSize,  halfSize,  halfSize}, // 前右
		{ halfSize,  halfSize, -halfSize}, // 后右
		{-halfSize,  halfSize, -halfSize}  // 后左
	};
	AddFaceVertices(vertices, topPos, normals[4]);

	// 下面 (Y-)
	const float bottomPos[4][3] = {
		{-halfSize, -halfSize, -halfSize}, // 后左
		{ halfSize, -halfSize, -halfSize}, // 后右
		{ halfSize, -halfSize,  halfSize}, // 前右
		{-halfSize, -halfSize,  halfSize}  // 前左
	};
	AddFaceVertices(vertices, bottomPos, normals[5]);

	return vertices;
}

// 生成索引数据
std::vector<unsigned int> CubeVerticesData::GenerateCubeIndices()
{
	std::vector<unsigned int> indices;

	// 每个面由2个三角形组成（6个索引）
	// 顶点顺序为逆时针（面向外面）
	for (unsigned int face = 0; face < 6; ++face) {
		unsigned int baseVertex = face * 4;

		// 第一个三角形
		indices.push_back(baseVertex);
		indices.push_back(baseVertex + 1);
		indices.push_back(baseVertex + 2);

		// 第二个三角形
		indices.push_back(baseVertex);
		indices.push_back(baseVertex + 2);
		indices.push_back(baseVertex + 3);
	}

	return indices;
}
void CubeVerticesData::AddFaceVertices(std::vector<float>& vertices,
	const float positions[4][3],
	const float normal[3])
{
	for (int i = 0; i < 4; ++i) {
		// 添加顶点位置 (x, y, z)
		vertices.push_back(positions[i][0]);
		vertices.push_back(positions[i][1]);
		vertices.push_back(positions[i][2]);

		// 添加法向量 (nx, ny, nz)
		vertices.push_back(normal[0]);
		vertices.push_back(normal[1]);
		vertices.push_back(normal[2]);
	}
}

#pragma once
#include "ModelDataInterFace.h"
class CubeVerticesData : public ModelDataInterFace
{
public:
    CubeVerticesData() = default;
    ~CubeVerticesData() = default;
    MeshData GetModelDatas() override;
private:
    std::vector<Vertex> GenerateCubeVertices(float size);
    std::vector<uint32_t> GenerateCubeIndices();
    void AddFaceVertices(std::vector<Vertex>& vertices, const glm::vec3 positions[4], const glm::vec3& normal);
};

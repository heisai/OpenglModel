#pragma once
#include "ModelDataInterFace.h"
class CylinderVerticesData : public ModelDataInterFace{
public:
    CylinderVerticesData() = default;
    ~CylinderVerticesData() = default;
    MeshData GetModelDatas() override;
private:
    std::vector<Vertex> GenerateCylinderVertices(float radius, float height, int sectors);
    std::vector<uint32_t> GenerateCylinderIndices(int sectors);
};

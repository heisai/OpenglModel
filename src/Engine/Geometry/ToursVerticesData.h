#pragma once
#include "ModelDataInterFace.h"
class ToursVerticesData  : public ModelDataInterFace
{
public:
    ToursVerticesData() = default;
    ~ToursVerticesData() = default;
    MeshData GetModelDatas() override;
private:
    std::vector<Vertex> GenerateTorusVertices(float majorRadius = 1.0f,
        float minorRadius = 0.4f,
        unsigned int majorSegments = 32,
        unsigned int minorSegments = 32 );
    std::vector<uint32_t> GenerateTorusIndices(unsigned int majorSegments,
        unsigned int minorSegments);
};

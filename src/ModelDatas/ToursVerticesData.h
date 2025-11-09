#pragma once
#include "ModelDataInterFace.h"
class ToursVerticesData  : public ModelDataInterFace
{
public:
    ToursVerticesData() = default;
    ~ToursVerticesData() = default;
    ModelDataInfo GetModelDatas() override;
private:
    std::vector<float> GenerateTorusVertices(float majorRadius = 1.0f,
        float minorRadius = 0.4f,
        unsigned int majorSegments = 32,
        unsigned int minorSegments = 16);
    std::vector<unsigned int> GenerateTorusIndices(unsigned int majorSegments = 32,
        unsigned int minorSegments = 16);
};

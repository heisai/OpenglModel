#pragma once
#include "ModelDataInterFace.h"
class CubeVerticesData : public ModelDataInterFace
{
public:
    CubeVerticesData() = default;
    ~CubeVerticesData() = default;
    ModelDataInfo GetModelDatas() override;
private:
    std::vector<float> GenerateCubeVertices(float size);
    std::vector<unsigned int> GenerateCubeIndices();
    void AddFaceVertices(std::vector<float>& vertices, const float positions[4][3], const float normal[3]);
};

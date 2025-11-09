#pragma once
#include "ModelDataInterFace.h"
class CylinderVerticesData : public ModelDataInterFace{
public:
    CylinderVerticesData() = default;
    ~CylinderVerticesData() = default;
    ModelDataInfo GetModelDatas() override;
private:
    std::vector<float> GenerateCylinderVertices(float radius, float height, int sectors);
    std::vector<unsigned int> GenerateCylinderIndices(int sectors);
};

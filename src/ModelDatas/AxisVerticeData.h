#pragma once
#include "ModelDataInterFace.h"
class AxisVerticeData : public ModelDataInterFace
{
public:
    AxisVerticeData() = default;
	~AxisVerticeData() = default;
    ModelDataInfo GetModelDatas() override;
private:
    std::vector<float>  GenerateAxisVertices();
    int gridSize = 50;
    int gridSize1 = 100;
    float gridSpacing = 0.25f;
};


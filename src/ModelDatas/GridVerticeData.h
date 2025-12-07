#pragma once
#include "ModelDataInterFace.h"
class GridVerticeData : public ModelDataInterFace 
{
public:
    GridVerticeData() = default;
	~GridVerticeData() = default;    
    ModelDataInfo GetModelDatas() override;
private:
    std::vector<float>  GenerateGridVertices();
    int gridSize = 50;
    int gridSize1 = 100;
    float gridSpacing = 0.25f;
};


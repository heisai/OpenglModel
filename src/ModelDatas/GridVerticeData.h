#pragma once
#include "ModelDataInterFace.h"
class GridVerticeData : public ModelDataInterFace 
{
public:
    GridVerticeData() = default;
	~GridVerticeData() = default;    
    MeshData GetModelDatas() override;
private:
    std::vector<Vertex> GenerateGridVertices();
    int gridSize = 50;
    int gridSize1 = 100;
    float gridSpacing = 0.25f;
};


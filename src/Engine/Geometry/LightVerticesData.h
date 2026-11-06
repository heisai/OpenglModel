#pragma once
#include "ModelDataInterFace.h"
class LightVerticesData : public ModelDataInterFace
{
public:
    LightVerticesData() = default;
    ~LightVerticesData () = default;
    MeshData GetModelDatas() override;
};

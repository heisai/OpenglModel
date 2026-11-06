#ifndef CREATMODELDATAIMPL_H
#define CREATMODELDATAIMPL_H
#include"CylinderVerticesData.h"
#include"ToursVerticesData.h"
#include"CubeVerticesData.h"
#include "GridVerticeData.h"
#include "AxisVerticeData.h"
#include"LightVerticesData.h"
class CreatModelDataImpl
{
public:
    CreatModelDataImpl() = default;
    MeshData GetModelDatas(OperatorAction type);

};

#endif // CREATMODELDATAIMPL_H

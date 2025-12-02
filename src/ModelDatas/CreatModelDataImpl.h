#ifndef CREATMODELDATAIMPL_H
#define CREATMODELDATAIMPL_H
#include"CylinderVerticesData.h"
#include"ToursVerticesData.h"
#include"CubeVerticesData.h"
class CreatModelDataImpl
{
public:
    CreatModelDataImpl();
    ModelDataInfo GetModelDatas(OperatorAction type);

};

#endif // CREATMODELDATAIMPL_H

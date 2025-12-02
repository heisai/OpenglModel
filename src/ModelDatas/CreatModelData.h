#ifndef CREATMODELDATA_H
#define CREATMODELDATA_H
#include "ModelDataInterFace.h"
class CreatModelDataImpl;
class CreatModelData
{
public:
    ModelDataInfo GetModelDatas(OperatorAction type);
};
#endif // CREATMODELDATA_H

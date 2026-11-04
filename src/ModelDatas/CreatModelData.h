#ifndef CREATMODELDATA_H
#define CREATMODELDATA_H
#include "ModelDataInterFace.h"
class CreatModelDataImpl;
class CreatModelData
{
public:
    MeshData GetModelDatas(OperatorAction type);
};
#endif // CREATMODELDATA_H

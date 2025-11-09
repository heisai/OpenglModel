#include "CreatModelDataImpl.h"

CreatModelDataImpl::CreatModelDataImpl() {}

ModelDataInfo CreatModelDataImpl::GetModelDatas(EngineType type)
{

    std::unique_ptr<ModelDataInterFace> model_data = nullptr;
    switch (type) {
    case EM_TOURSENGINE:
        model_data = move(std::make_unique<CylinderVerticesData>());
        break;
    case EM_CYLINDERENGINE:
        model_data = move(std::make_unique<ToursVerticesData>());
        break;
    default:
        break;
    }
    if(model_data)
    {
        return model_data->GetModelDatas();
    }
    else
    {
        return ModelDataInfo();
    }

}

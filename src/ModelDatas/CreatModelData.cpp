#include "CreatModelData.h"
#include "CreatModelDataImpl.h"
ModelDataInfo CreatModelData::GetModelDatas(EngineType type)
{
    std::unique_ptr<CreatModelDataImpl> model_data_ptr = std::make_unique<CreatModelDataImpl>();
    return model_data_ptr->GetModelDatas(type);
}

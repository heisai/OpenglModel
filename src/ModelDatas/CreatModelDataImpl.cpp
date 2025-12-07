#include "CreatModelDataImpl.h"

CreatModelDataImpl::CreatModelDataImpl() {}

ModelDataInfo CreatModelDataImpl::GetModelDatas(OperatorAction type)
{

    std::unique_ptr<ModelDataInterFace> model_data = nullptr;
    switch (type) {
    case OperatorAction::CreatCyliner:
        model_data = move(std::make_unique<CylinderVerticesData>());
        break;
    case OperatorAction::CreatTourse:
        model_data = move(std::make_unique<ToursVerticesData>());
        break;
    case OperatorAction::CreatCube:
        model_data = move(std::make_unique<CubeVerticesData>());
        break;
    case OperatorAction::CreatGrid:
        model_data = move(std::make_unique<GridVerticeData>());
        break;
	case OperatorAction::CreatAxis:
		model_data = move(std::make_unique<AxisVerticeData>());
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

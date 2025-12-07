#pragma once
#include"../Graphics.h"
class GridModel : public GraphicsEngine
{
public:
    explicit GridModel(OperatorAction type, std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;
};


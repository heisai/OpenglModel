#pragma once
#include"../Graphics.h"
class LightModel : public GraphicsEngine
{
public:
	explicit LightModel(OperatorAction type, std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;
};
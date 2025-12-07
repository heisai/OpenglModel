#pragma once
#include"../Graphics.h"
class AxisModel : public GraphicsEngine
{
public:
	explicit AxisModel(OperatorAction type, std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;




};

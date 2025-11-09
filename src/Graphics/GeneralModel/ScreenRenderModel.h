#pragma once
#include"../Graphics.h"
class ScreenRenderModel : public GraphicsEngine
{
public:
	explicit ScreenRenderModel(std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;
};

#pragma once
#include"../Graphics.h"
#include"../../Render/Texture2D.h"
class GridModel : public GraphicsEngine
{
public:
	explicit GridModel(OperatorAction type, std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;
private:
	Texture2D floor_texture_;
	Texture2D floor_texture_gamma_corrected_;
	bool gamma_enabled_;
};


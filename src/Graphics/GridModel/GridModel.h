#pragma once
#include"../Graphics.h"
class GridModel : public GraphicsEngine
{
public:
    explicit GridModel(OperatorAction type, std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;
private:
    unsigned int loadTexture(char const* path, bool gammaCorrection);
private:
	unsigned int floor_texture_;
	unsigned int floor_texture_gamma_corrected_;
	bool gamma_enabled_;
};


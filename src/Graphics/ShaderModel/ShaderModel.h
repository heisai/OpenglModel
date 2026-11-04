#pragma once
#include"../Graphics.h"
#include"../GeneralModel/ScreenRenderModel.h"
#include"../Texture2D.h"
#include <QElapsedTimer>
#include <filesystem>
#include <optional>
class ShaderModel : public GraphicsEngine
{
public:
	explicit ShaderModel(OperatorAction type, std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;
private:
	void syncTexture(Texture2D& texture, const std::optional<std::filesystem::path>& path, bool srgb);
	//模板测试  shader(用户物体选中)
	std::unique_ptr<Shader>stencil_shader_;
	QElapsedTimer elapsed_timer;
	Texture2D diffuse_texture_;
	Texture2D specular_texture_;
};

#pragma once
#include"../Graphics.h"
#include"../GeneralModel/ScreenRenderModel.h"
#include <QElapsedTimer>
class PlanetModel : public GraphicsEngine
{
public:
	explicit PlanetModel(OperatorAction type, std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;
private:
	//模板测试  shader(用户物体选中)
	std::unique_ptr<Shader>stencil_shader_;
	QElapsedTimer elapsed_timer;

	std::vector<glm::mat4 > model_matrices_;
};

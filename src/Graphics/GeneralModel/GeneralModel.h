#pragma once
#include"../Graphics.h"
#include"ScreenRenderModel.h"
class GeneralModel : public GraphicsEngine
{
public:
	explicit GeneralModel(OperatorAction type, std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;
    void SetLightColor(glm::vec3 lightcolor);
    void SetObjectColor(glm::vec3 objectcolor);
private:
    bool m_RayTrack = false;
    glm::vec3 m_ObjectColor = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 m_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	std::shared_ptr<Shader>base_shader_;



};

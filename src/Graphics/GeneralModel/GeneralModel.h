#pragma once
#include"../Graphics.h"
#include"ScreenRenderModel.h"
class GeneralModel : public GraphicsEngine
{
public:
	explicit GeneralModel(std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;
	void setModelData(const ModelDataInfo& datas) override;
    void SetLightColor(glm::vec3 lightcolor);
    void SetObjectColor(glm::vec3 objectcolor);
    void DrawStencil(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    bool colorPick(glm::mat4 model, glm::mat4 view, glm::mat4 projection, int xpos, int ypos,
        int objetc_id);
private:
    bool m_RayTrack = false;
    glm::vec3 m_ObjectColor = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 m_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    std::unique_ptr<ScreenRenderModel>screen_render_model_;
	unsigned int m_VBO, m_VAO, m_EBO;
	std::vector<float> m_Vertices;
	std::vector<unsigned int>m_Indices;

};

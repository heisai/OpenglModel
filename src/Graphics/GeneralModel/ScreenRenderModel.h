#pragma once
#include"../Graphics.h"
class ScreenRenderModel : public GraphicsEngine
{
public:
	explicit ScreenRenderModel(std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;
	void setModelData(const ModelDataInfo& datas) override;
	void setScreenRenderVertexData(unsigned int vao, std::vector<unsigned int>indices) override;


	bool colorPick(glm::mat4 model, glm::mat4 view, glm::mat4 projection, int readX, int readY,
		int objetc_id);
private:
	glm::vec3 idToColor(int id);
	int colorToId(const glm::vec3& color);
private:
	std::unique_ptr<Shader>screen_shader_;
	std::unique_ptr<Shader>color_pick_shader_;
	unsigned int default_render_vao_;
	std::vector<unsigned int>default_render_indices_;

	unsigned int m_PickFBO;
	unsigned int m_PickTexture;
	unsigned int m_PickRBO;

	unsigned int quadvao_;
	unsigned int quadvbo_;
};

#pragma once
#include"../Graphics.h"


class CubeMapsModel : public GraphicsEngine
{
public:
	explicit CubeMapsModel(std::shared_ptr<Shader> shader = nullptr);
	void Draw() override;
	void InitBufferData() override;   
	void setModelData(const ModelDataInfo& datas) override;
private:
	unsigned int loadCubemap(std::vector<std::string> faces);
	std::vector<std::string> faces = 
	{
		Utils::GlslAbsolute("right.jpg","CubeMapsModel\\skybox"),
		Utils::GlslAbsolute("left.jpg","CubeMapsModel\\skybox"),
		Utils::GlslAbsolute("top.jpg","CubeMapsModel\\skybox"),
		Utils::GlslAbsolute("bottom.jpg","CubeMapsModel\\skybox"),
		Utils::GlslAbsolute("front.jpg","CubeMapsModel\\skybox"),
		Utils::GlslAbsolute("back.jpg","CubeMapsModel\\skybox"),
	};
	std::vector<float> cubeVertices = {
  -1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	unsigned int textureid_;
	unsigned int m_VBO, m_VAO, m_EBO;
};

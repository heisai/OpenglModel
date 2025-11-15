#include"CubeMapsModel.h"
#define STB_IMAGE_IMPLEMENTATION
#include"../stb_image.h"
CubeMapsModel::CubeMapsModel(EngineType type, std::shared_ptr<Shader> shader /*= nullptr*/):
    GraphicsEngine(type,shader)
{
	// 使用拾取着色器

}


void CubeMapsModel::Draw()
{
	glDepthFunc(GL_LEQUAL);

	m_shader->bind();
	glm::mat4 view_ = glm::translate(mvp_data_->view_, glm::vec3(0.0f, 0.0f, 10.0f));
	m_shader->setMat4("view", view_);
	m_shader->setMat4("projection", mvp_data_->projection_);
	// cubes
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid_);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);


}

void CubeMapsModel::InitBufferData()
{

	m_shader->CreatProgram();

	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// 顶点数组对象
	glGenVertexArrays(1, &m_VAO);
	//绑定VAO
	glBindVertexArray(m_VAO);

	//把顶点数组复制到缓冲中供OpenGL使用
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), cubeVertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	textureid_ = loadCubemap(faces);

	m_shader->SetInt("skybox", 0);
}



unsigned int CubeMapsModel::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
#if 1
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	for (int i = 0; i < faces.size(); i++)
	{
		LogInfo("jpd:{}",faces[i].c_str());
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif
	return textureID;
}


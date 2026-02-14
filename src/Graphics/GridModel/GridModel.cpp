#include"GridModel.h"

GridModel::GridModel(OperatorAction type, std::shared_ptr<Shader> shader /*= nullptr*/):
	GraphicsEngine(type, shader),
	floor_texture_(0),
	floor_texture_gamma_corrected_(0),
	gamma_enabled_(false)
{

}

void GridModel::Draw()
{		
	default_shader_->bind();

	mvp_data_->model_ = glm::rotate(glm::mat4(1.0f), glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	default_shader_->setMat4("view", mvp_data_->view_);
	default_shader_->setMat4("model", mvp_data_->model_);
	default_shader_->setMat4("projection", mvp_data_->projection_);

	default_shader_->setMat4("lightPositions", mvp_data_->projection_);
	default_shader_->setMat4("lightColors", mvp_data_->projection_);

	glBindVertexArray(mesh_data_->vao_);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gamma_enabled_ ? floor_texture_gamma_corrected_ : floor_texture_);
	glDrawArrays(GL_TRIANGLES, 0, 6);


}

void GridModel::InitBufferData()
{
	if (default_shader_)
	{
		default_shader_->CreatProgram();
	}
	// 顶点数组对象
	glGenVertexArrays(1, &mesh_data_->vao_);
	glBindVertexArray(mesh_data_->vao_);
	//顶点缓冲对象
	glGenBuffers(1, &mesh_data_->vbo_);

	//把顶点数组复制到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, mesh_data_->vbo_);
	glBufferData(GL_ARRAY_BUFFER, mesh_data_->vertices_datas.size() * sizeof(float), mesh_data_->vertices_datas.data(), GL_STATIC_DRAW);

	//设置顶点属性指针
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	unsigned int floorTexture = loadTexture(Utils::joinPaths("GridModel", "wood.png").c_str(), false);
	unsigned int floorTextureGammaCorrected = loadTexture(Utils::joinPaths("GridModel", "wood.png").c_str(), true);

	// shader configuration
	// --------------------
	default_shader_->bind();
	default_shader_->SetInt("floorTexture", 0);

}

unsigned int GridModel::loadTexture(char const* path, bool gammaCorrection)
{
	LogInfo("Loading texture at path: {}", path);
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		if (nrComponents == 1)
		{
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

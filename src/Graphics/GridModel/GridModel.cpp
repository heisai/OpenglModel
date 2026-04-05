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

	default_shader_->setMat4("ViewMatrix", mvp_data_->view_);
	default_shader_->setMat4("ProjectionMatrix", mvp_data_->projection_);
	default_shader_->setMat4("ModelMatrix", mvp_data_->model_);

	// viewPos 必须是相机世界坐标（从 view 矩阵求逆得到）
	glm::mat4 invView = glm::inverse(mvp_data_->view_);
	glm::vec3 camPosWorld = glm::vec3(invView[3]);
	default_shader_->setVec3("viewPos", camPosWorld);

	// Light / Material
	default_shader_->SetInt("Light.Type", 0);                         // 0=方向光 1=点光源 2=聚光灯
	default_shader_->setVec3("Light.Position", 0.2f, 5.0f, -2.0f);  // 方向光：此字段作为光线方向使用
	default_shader_->setVec3("Light.La", 0.2f, 0.2f, 0.2f);
	default_shader_->setVec3("Light.Ld", 0.5f, 0.5f, 0.5f);
	default_shader_->setVec3("Light.Ls", 1.0f, 1.0f, 1.0f);
	gamma_enabled_ = true;
	default_shader_->setBool("gamma", gamma_enabled_);
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	floor_texture_ = loadTexture(Utils::joinPaths("GridModel", "wood.png").c_str(), false);
	floor_texture_gamma_corrected_ = loadTexture(Utils::joinPaths("GridModel", "wood.png").c_str(), true);

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


		glBindTexture(GL_TEXTURE_2D, textureID);
		

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

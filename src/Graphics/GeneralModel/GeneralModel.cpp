#include"GeneralModel.h"
GeneralModel::GeneralModel(OperatorAction type, std::shared_ptr<Shader> shader /*= nullptr*/):
    GraphicsEngine(type,shader),
	m_RayTrack(false)
{
	stencil_shader_ = std::make_shared<Shader>("stencil_vertex.vs", "stencil_fragment.fs", "GeneralModel");
}


void GeneralModel::Draw()
{
	//if (selected_)
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	}
	default_shader_->bind();
	default_shader_->setMat4("view", mvp_data_->view_);
	default_shader_->setMat4("projection", mvp_data_->projection_);
	default_shader_->setMat4("model", mvp_data_->model_);


	//设置光源属性
	default_shader_->setVec3("viewPos", mvp_data_->view_[3]);
	default_shader_->setVec3("light.positiom", 1.2f, 1.0f, 2.0f);
	default_shader_->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	default_shader_->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	default_shader_->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//设置材质属性
	default_shader_->setVec3("material.ambient", 1.2f, 1.0f, 2.0f);
	default_shader_->setVec3("material.diffuse", 0.2f, 0.2f, 0.2f);
	default_shader_->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	default_shader_->setFloat("material.shininess", 32.0f);
	glBindVertexArray(mesh_data_->vao_);
	glDrawElements(GL_TRIANGLES, mesh_data_->indices_datas.size(), GL_UNSIGNED_INT, 0);


	if (selected_)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		stencil_shader_->bind();
		float scale = 1.025f;
		glm::mat4 model_ = glm::scale(mvp_data_->model_, glm::vec3(scale, scale, scale));
		stencil_shader_->setMat4("view", mvp_data_->view_);
		stencil_shader_->setMat4("projection", mvp_data_->projection_);
		stencil_shader_->setMat4("model", model_);
		glBindVertexArray(mesh_data_->vao_);
		glDrawElements(GL_TRIANGLES, mesh_data_->indices_datas.size(), GL_UNSIGNED_INT, 0);

	}
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);



}

void GeneralModel::InitBufferData()
{
	
	if (default_shader_)
	{
		default_shader_->CreatProgram();
	}
	if (m_PickShader)
	{
		m_PickShader->CreatProgram();
	}
	if (stencil_shader_)
	{
		stencil_shader_->CreatProgram();
	}


	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_STENCIL_TEST);
	//glEnable(GL_MULTISAMPLE);
	//glHint(GL_LINE_SMOOTH, GL_NICEST);

	// 顶点数组对象
	glGenVertexArrays(1, &mesh_data_->vao_);
	//绑定VAO
	glBindVertexArray(mesh_data_->vao_);

	//把顶点数组复制到缓冲中供OpenGL使用
	glGenBuffers(1, &mesh_data_->vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_data_->vbo_);
	glBufferData(GL_ARRAY_BUFFER, mesh_data_->vertices_datas.size() * sizeof(float), mesh_data_->vertices_datas.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &mesh_data_->ebo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data_->ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_data_->indices_datas.size() * sizeof(unsigned int), mesh_data_->indices_datas.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//stencil_shader_->CreatProgram();	
}

void GeneralModel::SetLightColor(glm::vec3 lightcolor)
{
	m_LightColor = lightcolor;
}

void GeneralModel::SetObjectColor(glm::vec3 objectcolor)
{
	m_ObjectColor = objectcolor;
}



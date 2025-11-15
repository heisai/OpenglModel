#include"GeneralModel.h"
GeneralModel::GeneralModel(std::shared_ptr<Shader> shader /*= nullptr*/):
    GraphicsEngine(shader),
	m_RayTrack(false)
{

	// 使用拾取着色器
	//m_PickShader = std::make_shared<Shader>("pick_vertex.vs", "pick_fragment.fs", "GeneralModel");
	stencil_shader_ = std::make_shared<Shader>("stencil_vertex.vs", "stencil_fragment.fs", "GeneralModel");
	screen_render_model_ = std::make_unique<ScreenRenderModel>(shader);

}


void GeneralModel::Draw()
{

	/*glViewport(0, 0, m_Width, m_Height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);*/
	/*glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
	
	if (selected_)
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	}
	m_shader->bind();
	m_shader->setMat4("view", mvp_data_->view_);
	m_shader->setMat4("projection", mvp_data_->projection_);
	m_shader->setMat4("model", mvp_data_->model_);


	//设置光源属性
	m_shader->setVec3("viewPos", mvp_data_->view_[3]);
	m_shader->setVec3("light.positiom", 1.2f, 1.0f, 2.0f);
	m_shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	m_shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	m_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//设置材质属性
	m_shader->setVec3("material.ambient", 1.2f, 1.0f, 2.0f);
	m_shader->setVec3("material.diffuse", 0.2f, 0.2f, 0.2f);
	m_shader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	m_shader->setFloat("material.shininess", 32.0f);
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	if (selected_)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		stencil_shader_->bind();
		float scale = 1.025f;
		stencil_shader_->setMat4("view", mvp_data_->view_);
		stencil_shader_->setMat4("projection", mvp_data_->projection_);
		stencil_shader_->setMat4("model", mvp_data_->model_);
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

		
	}

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
	

	//screen_render_model_->Draw();

}

void GeneralModel::InitBufferData()
{
	
	if (m_shader)
	{
		m_shader->CreatProgram();
	}
	if (m_PickShader)
	{
		m_PickShader->CreatProgram();
	}
	if (stencil_shader_)
	{
		stencil_shader_->CreatProgram();
	}


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_MULTISAMPLE);
	glHint(GL_LINE_SMOOTH, GL_NICEST);

	// 顶点数组对象
	glGenVertexArrays(1, &m_VAO);
	//绑定VAO
	glBindVertexArray(m_VAO);

	//把顶点数组复制到缓冲中供OpenGL使用
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	stencil_shader_->CreatProgram();
	screen_render_model_->setScreenRenderVertexData(m_VAO, m_Indices);
	screen_render_model_->SetViewSize(m_Width, m_Height);
	screen_render_model_->InitBufferData();
	
}

void GeneralModel::setModelData(const ModelDataInfo& datas)
{
	m_Vertices = datas.m_Vertices;
	m_Indices = datas.m_Indices;
}

void GeneralModel::SetLightColor(glm::vec3 lightcolor)
{
	m_LightColor = lightcolor;
}

void GeneralModel::SetObjectColor(glm::vec3 objectcolor)
{
	m_ObjectColor = objectcolor;
}

void GeneralModel::DrawStencil(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
#if 0
	
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	m_Stencilshader->UseProgram();
	float scale = 1.025f;
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	m_Stencilshader->setMat4("model", model);
	m_Stencilshader->setMat4("view", view);
	m_Stencilshader->setMat4("projection", projection);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
#endif 
}

bool GeneralModel::colorPick(glm::mat4 model, glm::mat4 view, glm::mat4 projection, int readX, int readY,
	int objetc_id)
{

	

	selected_ = screen_render_model_->colorPick(model, view, projection, readX, readY, objetc_id);
	return selected_;
	LogInfo( "========================== START PICKING =======================");
}



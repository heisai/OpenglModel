#include"ShaderModel.h"
ShaderModel::ShaderModel(OperatorAction type, std::shared_ptr<Shader> shader /*= nullptr*/):
    GraphicsEngine(type,shader)
{
	stencil_shader_ = std::make_unique<Shader>("stencil_vertex.vs", "stencil_fragment.fs", "GeneralModel");
	elapsed_timer.start();

}


void ShaderModel::Draw()
{
	//if (selected_)
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	}
	
	default_shader_->bind();

	// mat3 => 用 setMat3
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(mvp_data_->model_)));
	default_shader_->setMat3("NormalMatrix", normalMatrix);

	default_shader_->setMat4("ViewMatrix", mvp_data_->view_);
	default_shader_->setMat4("ProjectionMatrix", mvp_data_->projection_);
	default_shader_->setMat4("ModelMatrix", mvp_data_->model_);

	// viewPos 必须是相机世界坐标（从 view 矩阵求逆得到）
	glm::mat4 invView = glm::inverse(mvp_data_->view_);
	glm::vec3 camPosWorld = glm::vec3(invView[3]);
	default_shader_->setVec3("viewPos", camPosWorld);

	// Light / Material
	default_shader_->setVec3("Light.Position", 1.2f, 1.0f, 2.0f);
	default_shader_->setVec3("Light.La", 0.2f, 0.2f, 0.2f);
	default_shader_->setVec3("Light.Ld", 0.5f, 0.5f, 0.5f);
	default_shader_->setVec3("Light.Ls", 1.0f, 1.0f, 1.0f);

	//default_shader_->setVec3("Material.Ka", 1.2f, 1.0f, 2.0f);
	//default_shader_->setVec3("Material.Kd", 0.2f, 0.2f, 0.2f);
	//default_shader_->setVec3("Material.Ks", 0.5f, 0.5f, 0.5f);
	//default_shader_->setFloat("Material.Shininess", 32.0f);

	default_shader_->setMaterial(materal_);





	//default_shader_->setBool("geom_flag", true);
	default_shader_->setFloat("time", static_cast<float>(elapsed_timer.elapsed()/500));
	default_shader_->setBool("geom_flag", mvp_data_->blasting_flag_);
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

void ShaderModel::InitBufferData()
{
	
	if (default_shader_)
	{
		default_shader_->CreatProgram();
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


#include"PlanetModel.h"
PlanetModel::PlanetModel(OperatorAction type, std::shared_ptr<Shader> shader /*= nullptr*/) :
	GraphicsEngine(type, shader)
{
	stencil_shader_ = std::make_unique<Shader>("stencil_vertex.vs", "stencil_fragment.fs", "GeneralModel");
	elapsed_timer.start();





	unsigned int amount = 1000;

	srand(static_cast<unsigned int>(static_cast<float>(elapsed_timer.elapsed()))); // initialize random seed

	float radius = 150.0;
	float offset = 25.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = static_cast<float>((rand() % 360));
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		model_matrices_.push_back(model) ;
	}



}


void PlanetModel::Draw()
{

		//if (selected_)s
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


		//default_shader_->setBool("geom_flag", true);
		default_shader_->setFloat("time", static_cast<float>(elapsed_timer.elapsed() / 500));
		default_shader_->setBool("geom_flag", mvp_data_->blasting_flag_);
		glBindVertexArray(mesh_data_->vao_);
		//glDrawElements(GL_TRIANGLES, mesh_data_->indices_datas.size(), GL_UNSIGNED_INT, 0);
		glDrawElementsInstanced(GL_TRIANGLES, mesh_data_->indices_datas.size(), GL_UNSIGNED_INT, 0, model_matrices_.size());




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

void PlanetModel::InitBufferData()
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



	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * model_matrices_.size(), model_matrices_.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);
}


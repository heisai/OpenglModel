#include"GridModel.h"

GridModel::GridModel(OperatorAction type, std::shared_ptr<Shader> shader /*= nullptr*/):
	GraphicsEngine(type, shader)
{

}

void GridModel::Draw()
{
	default_shader_->bind();
	default_shader_->setMat4("view", mvp_data_->view_);
	default_shader_->setMat4("projection", mvp_data_->projection_);
	default_shader_->setMat4("model", mvp_data_->model_);
	glBindVertexArray(mesh_data_->vao_);
	glLineWidth(1.0f); // 
	glDrawArrays(GL_LINES, 0, mesh_data_->vertices_datas.size() / 3);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

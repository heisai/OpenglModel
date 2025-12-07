#include"AxisModel.h"
AxisModel::AxisModel(OperatorAction type, std::shared_ptr<Shader> shader /*= nullptr*/):
    GraphicsEngine(type,shader)
{
	
}


void AxisModel::Draw()
{
	glPointSize(100.0);
	//if (selected_)
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	}
	default_shader_->bind();
	default_shader_->setMat4("view", mvp_data_->view_);
	default_shader_->setMat4("projection", mvp_data_->projection_);
	default_shader_->setMat4("model", mvp_data_->model_);



	//if (selected_)
	{
		
		glBindVertexArray(mesh_data_->vao_);
		glDrawArrays(GL_POINTS, 0, 1); // 绘制一个点作为坐标轴的起点
	}
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);



}

void AxisModel::InitBufferData()
{
	
	if (default_shader_)
	{
		default_shader_->CreatProgram();
	}
	// 顶点数组对象
	glGenVertexArrays(1, &mesh_data_->vao_);
	//绑定VAO
	glBindVertexArray(mesh_data_->vao_);

	//把顶点数组复制到缓冲中供OpenGL使用
	glGenBuffers(1, &mesh_data_->vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_data_->vbo_);
	glBufferData(GL_ARRAY_BUFFER, mesh_data_->vertices_datas.size() * sizeof(float), mesh_data_->vertices_datas.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}




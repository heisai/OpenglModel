#include"GridModel.h"
#include <QDebug>
#include <QString>

GridModel::GridModel(OperatorAction type, std::shared_ptr<Shader> shader /*= nullptr*/):
	GraphicsEngine(type, shader),
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

	gamma_enabled_ = true;
	default_shader_->setBool("gamma", gamma_enabled_);
	glBindVertexArray(mesh_data_->vao_);

	Texture2D& texture = gamma_enabled_ ? floor_texture_gamma_corrected_ : floor_texture_;
	if (!texture.bind(GL_TEXTURE0))
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
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
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mesh_data_->data_.vertices.size() * sizeof(Vertex)), mesh_data_->data_.vertices.data(), GL_STATIC_DRAW);

	//设置顶点属性指针

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(Vertex)), reinterpret_cast<const void*>(offsetof(Vertex, position)));
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(Vertex)), reinterpret_cast<const void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(Vertex)), reinterpret_cast<const void*>(offsetof(Vertex, texCoord)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	const std::filesystem::path texturePath = Utils::joinPaths("GridModel", "wood.png");
	std::string error;
	if (!floor_texture_.loadFromFile(texturePath, false, error))
	{
		qWarning().noquote() << QString::fromStdString(error);
	}
	if (!floor_texture_gamma_corrected_.loadFromFile(texturePath, true, error))
	{
		qWarning().noquote() << QString::fromStdString(error);
	}

	// shader configuration
	// --------------------
	default_shader_->bind();
	default_shader_->SetInt("floorTexture", 0);

}

#include "LightModel.h"
#include <span>

LightModel::LightModel(OperatorAction type, std::shared_ptr<Shader> shader):
	GraphicsEngine(type, shader)
{

}

void LightModel::Draw()
{

	default_shader_->bind();
	default_shader_->setMat4("view", mvp_data_->view_);
	default_shader_->setMat4("projection", mvp_data_->projection_);
	default_shader_->setMat4("model", mvp_data_->model_);

	//设置光源属性
	default_shader_->setVec3("viewPos", glm::vec3(glm::inverse(mvp_data_->view_)[3]));
	glBindVertexArray(mesh_data_->vao_);
	
	// ② 显式窄化转换：size_t → GLsizei
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh_data_->data_.indices.size()), GL_UNSIGNED_INT, nullptr);

}

void LightModel::InitBufferData()
{
	// ④ [[likely]]：shader 通常非空
	if (default_shader_) [[likely]]
	{
		default_shader_->CreatProgram();
	}

	// 顶点数组对象
	glGenVertexArrays(1, &mesh_data_->vao_);
	glBindVertexArray(mesh_data_->vao_);

	// ⑤ std::span::size_bytes()：替代手动 size() * sizeof(T)，避免类型计算出错
	glGenBuffers(1, &mesh_data_->vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_data_->vbo_);
	{
		const std::span vertSpan{ mesh_data_->data_.vertices };
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertSpan.size_bytes()), vertSpan.data(), GL_STATIC_DRAW);
	}

	glGenBuffers(1, &mesh_data_->ebo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data_->ebo_);
	{
		const std::span idxSpan{ mesh_data_->data_.indices };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(idxSpan.size_bytes()), idxSpan.data(), GL_STATIC_DRAW);
	}

	// ⑥ nullptr 替代 (void*)0；reinterpret_cast 替代 C 风格指针转型
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE,
		static_cast<GLsizei>(sizeof(Vertex)),
		reinterpret_cast<const void*>(offsetof(Vertex, position)));
	glEnableVertexAttribArray(0);
}

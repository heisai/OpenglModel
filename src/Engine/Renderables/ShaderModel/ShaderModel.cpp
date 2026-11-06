#include"ShaderModel.h"
#include <QDebug>
#include <QString>

ShaderModel::ShaderModel(OperatorAction type, std::shared_ptr<Shader> shader /*= nullptr*/):
    GraphicsEngine(type,shader)
{
	stencil_shader_ = std::make_unique<Shader>("stencil_vertex.vs", "stencil_fragment.fs", "GeneralModel");
	elapsed_timer.start();

}

void ShaderModel::syncTexture(Texture2D& texture, const std::optional<std::filesystem::path>& path, bool srgb)
{
	if (!path)
	{
		texture.release();
		return;
	}

	if (texture.sourcePath() == path
		&& (texture.isLoadedForCurrentContext() || texture.loadFailedForCurrentContext()))
	{
		return;
	}
	if (!texture.release())
	{
		return;
	}

	std::string error;
	if (!texture.loadFromFile(*path, srgb, error))
	{
		qWarning().noquote() << QString::fromStdString(error);
	}
}

void ShaderModel::Draw()
{
	syncTexture(diffuse_texture_, materal_.diffuse_texture_path_, true);
	syncTexture(specular_texture_, materal_.specular_texture_path_, false);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	
	default_shader_->bind();

	default_shader_->setMat4("ViewMatrix", mvp_data_->view_);
	default_shader_->setMat4("ProjectionMatrix", mvp_data_->projection_);
	default_shader_->setMat4("ModelMatrix", mvp_data_->model_);

	// viewPos 必须是相机世界坐标（从 view 矩阵求逆得到）
	glm::mat4 invView = glm::inverse(mvp_data_->view_);
	glm::vec3 camPosWorld = glm::vec3(invView[3]);
	default_shader_->setVec3("viewPos", camPosWorld);

	// Material is per model; lights are provided by the scene.
	default_shader_->setMaterial(materal_,
		diffuse_texture_.isLoadedForCurrentContext(),
		specular_texture_.isLoadedForCurrentContext());
	if (!diffuse_texture_.bind(GL_TEXTURE0))
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	if (!specular_texture_.bind(GL_TEXTURE1))
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);

	

	////default_shader_->setBool("geom_flag", true);
	//default_shader_->setFloat("time", static_cast<float>(elapsed_timer.elapsed()/500));
	//default_shader_->setBool("geom_flag", mvp_data_->blasting_flag_);
	
	glBindVertexArray(mesh_data_->vao_);
	glDrawElements(GL_TRIANGLES, mesh_data_->data_.indices.size(), GL_UNSIGNED_INT, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);


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
		glDrawElements(GL_TRIANGLES, mesh_data_->data_.indices.size(), GL_UNSIGNED_INT, 0);
	}
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	// 顶点数组对象
	glGenVertexArrays(1, &mesh_data_->vao_);
	//绑定VAO
	glBindVertexArray(mesh_data_->vao_);

	//把顶点数组复制到缓冲中供OpenGL使用
	glGenBuffers(1, &mesh_data_->vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_data_->vbo_);
	glBufferData(GL_ARRAY_BUFFER, mesh_data_->data_.vertices.size() * sizeof(Vertex), mesh_data_->data_.vertices.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &mesh_data_->ebo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data_->ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_data_->data_.indices.size() * sizeof(uint32_t), mesh_data_->data_.indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(Vertex)), reinterpret_cast<const void*>(offsetof(Vertex, position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(Vertex)), reinterpret_cast<const void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(Vertex)), reinterpret_cast<const void*>(offsetof(Vertex, texCoord)));
	glEnableVertexAttribArray(2);
	//设置单面着色
	//glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
	//stencil_shader_->CreatProgram();	
}



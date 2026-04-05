#include "GeneralModel.h"
#include <span>

namespace {
	constexpr float    kOutlineScale  = 1.025f;
	constexpr float    kTimeScale     = 500.0f;
	constexpr GLsizei  kVertexStride  = 6 * static_cast<GLsizei>(sizeof(float));
	constexpr GLintptr kNormalOffset  = 3 * static_cast<GLintptr>(sizeof(float));
}
/// <summary>
/// 构造一个 GeneralModel 对象，初始化图形引擎和模板着色器。
/// </summary>
/// <param name="type">操作类型。</param>
/// <param name="shader">着色器的共享指针，默认为 nullptr。</param>
GeneralModel::GeneralModel(OperatorAction type, std::shared_ptr<Shader> shader /*= nullptr*/):
	GraphicsEngine(type,shader)
{
	stencil_shader_ = std::make_unique<Shader>("stencil_vertex.vs", "stencil_fragment.fs", "GeneralModel");
	elapsed_timer.start();
}

/// <summary>
/// 渲染模型，包含光照着色与可选的模板描边效果。
///
/// 渲染流程分为两个阶段：
///
/// **阶段一：主体渲染**
/// - 配置模板测试为始终通过（GL_ALWAYS），并将通过的片段写入模板缓冲区（值为 1）。
/// - 绑定默认着色器，上传 MVP 矩阵（model / view / projection）。
/// - 设置光源属性（位置、环境光、漫反射、镜面反射）与材质属性（同上 + 光泽度）。
/// - 上传经过时间（用于顶点动画）与几何爆炸标志（geom_flag）。
/// - 调用 glDrawElements 以三角形图元完成索引绘制。
///
/// **阶段二：描边渲染（仅当 selected_ 为 true 时执行）**
/// - 该分支以 [[unlikely]] 标注，为小概率路径。
/// - 将模板测试改为 GL_NOTEQUAL，使描边着色器仅绘制主体之外的像素。
/// - 禁用深度测试，确保描边始终可见。
/// - 使用 kOutlineScale 对模型矩阵整体缩放，生成略大于原模型的轮廓壳。
/// - 绑定 stencil_shader_ 并重新上传 MVP，绘制描边几何体。
///
/// 渲染结束后恢复模板缓冲写入权限、重置模板测试条件，并重新启用深度测试，
/// 以保证后续渲染管线状态正确。
/// </summary>
void GeneralModel::Draw()
{
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
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

	// ① 修复：先转 float 再除，避免 qint64 整除丢失小数精度
	default_shader_->setFloat("time", static_cast<float>(elapsed_timer.elapsed()) / kTimeScale);
	default_shader_->setBool("geom_flag", mvp_data_->blasting_flag_);
	glBindVertexArray(mesh_data_->vao_);
	// ② 显式窄化转换：size_t → GLsizei
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh_data_->indices_datas.size()), GL_UNSIGNED_INT, nullptr);

	// ③ [[unlikely]]：选中是小概率分支
	if (selected_) [[unlikely]]
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		stencil_shader_->bind();
		const glm::mat4 scaledModel = glm::scale(mvp_data_->model_, glm::vec3(kOutlineScale));
		stencil_shader_->setMat4("view", mvp_data_->view_);
		stencil_shader_->setMat4("projection", mvp_data_->projection_);
		stencil_shader_->setMat4("model", scaledModel);
		glBindVertexArray(mesh_data_->vao_);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh_data_->indices_datas.size()), GL_UNSIGNED_INT, nullptr);
	}
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_DEPTH_TEST);
}

void GeneralModel::InitBufferData()
{
	// ④ [[likely]]：shader 通常非空
	if (default_shader_) [[likely]]
	{
		default_shader_->CreatProgram();
	}
	if (stencil_shader_) [[likely]]
	{
		stencil_shader_->CreatProgram();
	}

	// 顶点数组对象
	glGenVertexArrays(1, &mesh_data_->vao_);
	glBindVertexArray(mesh_data_->vao_);

	// ⑤ std::span::size_bytes()：替代手动 size() * sizeof(T)，避免类型计算出错
	glGenBuffers(1, &mesh_data_->vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_data_->vbo_);
	{
		const std::span vertSpan{ mesh_data_->vertices_datas };
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertSpan.size_bytes()), vertSpan.data(), GL_STATIC_DRAW);
	}

	glGenBuffers(1, &mesh_data_->ebo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data_->ebo_);
	{
		const std::span idxSpan{ mesh_data_->indices_datas };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(idxSpan.size_bytes()), idxSpan.data(), GL_STATIC_DRAW);
	}

	// ⑥ nullptr 替代 (void*)0；reinterpret_cast 替代 C 风格指针转型
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, kVertexStride, nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, kVertexStride, reinterpret_cast<const void*>(kNormalOffset));
	glEnableVertexAttribArray(1);
}



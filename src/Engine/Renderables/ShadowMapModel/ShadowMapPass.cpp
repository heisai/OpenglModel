#include "ShadowMapPass.h"
#include <QDebug>
#include <cmath>

namespace
{
// Reject invalid transforms before submitting any geometry.
bool finiteMatrix(const glm::mat4 &matrix)
{
	for (int column = 0; column < 4; ++column)
	{
		for (int row = 0; row < 4; ++row)
		{
			if (!std::isfinite(matrix[column][row]))
			{
				return false;
			}
		}
	}
	return true;
}
}

ShadowMapPass::ShadowMapPass(OperatorAction type, std::shared_ptr<Shader> shader)
	: GraphicsEngine(type, shader)
{
	if (!default_shader_)
	{
		default_shader_ = std::make_shared<Shader>("shadow_depth.vert", "shadow_depth.frag", "ShadowMapModel");
	}
	m_Width = 2048;
	m_Height = 2048;
}

ShadowMapPass::ShadowMapPass(OperatorAction type)
	: ShadowMapPass(type, nullptr)
{
}

ShadowMapPass::~ShadowMapPass()
{

}

void ShadowMapPass::InitBufferData()
{
	if (!default_shader_)
	{
		default_shader_ = std::make_shared<Shader>("shadow_depth.vert", "shadow_depth.frag", "ShadowMapModel");
	}
	default_shader_->CreatProgram();
	if (default_shader_->ShaderPromger == 0)
	{
		qWarning() << "ShadowMapPass: depth shader program creation failed";
		return;
	}

	//创建缓冲区对象
	glGenFramebuffers(1, &depth_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
	//创建纹理
	glGenTextures(1, &depth_texture);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,m_Width,m_Height,0,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//把深度纹理作为缓冲区的深度缓冲
	
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depth_texture,0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		qWarning() << "ShadowMapPass: framebuffer is not complete";
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);


}


void ShadowMapPass::Draw()
{
	//必要参数判断
	if (!default_shader_ || default_shader_->ShaderPromger == 0 ||
		!caster_mvp_data_ || !caster_mesh_ || depth_fbo == 0)
	{
		return;
	}
	//设置着色器参数（前提必须要绑定着色器:glUseProgram(ShaderPromger);）
	default_shader_->bind();	
	default_shader_->setMat4("lightSpaceMatrix", light_view_projection_);
	default_shader_->setMat4("model", caster_mvp_data_->model_);

	//绘制模型
	glBindVertexArray(caster_mesh_->vao_);
	unsigned int render_indices_size = static_cast<unsigned int>(caster_mesh_->data_.indices.size());
	unsigned int instance_count_ = static_cast<unsigned int>(instances_count_);
	unsigned int vertex_count_ = static_cast<unsigned int>(caster_mesh_->data_.vertices.size());
	const bool instanced = instance_count_ > 0;
	default_shader_->setBool("instanced", instanced);

	if (instance_count_ > 0)
	{
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(render_indices_size), GL_UNSIGNED_INT,
			nullptr, instance_count_);
	}
	else if (instance_count_ == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(render_indices_size), GL_UNSIGNED_INT, nullptr);
	}
	glBindVertexArray(0);

}

void ShadowMapPass::setRenderData(const MeshPtr& render_mesh_data, const MvpDataPtr& render_mvp_data,int instances)
{
	caster_mesh_ = render_mesh_data;
	caster_mvp_data_ = render_mvp_data;
	instances_count_ = instances;
}

void ShadowMapPass::beginRenderPass()
{
	glViewport(0, 0, m_Width, m_Height);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMapPass::endRenderPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_Width, m_Height);
}





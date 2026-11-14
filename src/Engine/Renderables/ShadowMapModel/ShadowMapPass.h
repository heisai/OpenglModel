#pragma once
#include "../Graphics.h"
#include <functional>

// Borrowed geometry; its VAO and instance attributes must remain alive during Draw().
struct ShadowDepthGeometry
{
	GLuint vao = 0;
	GLsizei vertexCount = 0;
	GLsizei indexCount = 0;
	GLsizei instanceCount = 0;
	glm::mat4 model{1.0f};
};

class ShadowMapPass : public GraphicsEngine
{
  public:
	// Reuse the existing Shader wrapper and pass in caller-provided depth shaders when available.
	explicit ShadowMapPass(OperatorAction type);
	ShadowMapPass(OperatorAction type, std::shared_ptr<Shader> shader);
	~ShadowMapPass();
	//初始化缓冲区数据
	void InitBufferData() override;
	// 设置渲染数据
	void Draw() override;
	// 设置渲染数据
 	void setRenderData(const MeshPtr& render_mesh_data,const MvpDataPtr& render_mvp_data,int instances) override;
	// 渲染前的操作
	void beginRenderPass() override;
	// 渲染后操作
	void endRenderPass() override;

  private:
	GLuint depth_fbo = 0;
	GLuint depth_texture = 0;
	glm::mat4 light_view_projection_{1.0f};
	MeshPtr caster_mesh_;
	MvpDataPtr caster_mvp_data_;
	int  instances_count_ = 0;
};
#include"ScreenRenderModel.h"
ScreenRenderModel::ScreenRenderModel(std::shared_ptr<Shader> shader /*= nullptr*/):
    GraphicsEngine(shader)
{
	// 使用拾取着色器
	m_PickShader = std::make_shared<Shader>("pick_vertex.vs", "pick_fragment.fs", "GeneralModel");
	stencil_shader_ = std::make_shared<Shader>("stencil_vertex.vs", "stencil_fragment.fs", "GeneralModel");
	screen_shader_ = std::make_shared<Shader>("fbo_vertex.vs", "fbo_fragment.fs", "GeneralModel");

}


void ScreenRenderModel::Draw()
{


#if 1
	//
	LogInfo("glGetError():{}" ,int(glGetError()));
	glViewport(0, 0, m_Width, m_Height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.9f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	LogInfo("Width :{} Height: {}", m_Width, m_Height);
	
	//if (selected_)
	//{
	//	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//	glStencilMask(0xFF);
	//}
	m_shader->UseProgram();
	m_shader->setMat4("view", mvp_data_->view_);
	m_shader->setMat4("projection", mvp_data_->projection_);
	m_shader->setMat4("model", mvp_data_->model_);


	//设置光源属性
	m_shader->setVec3("viewPos", mvp_data_->view_[3]);
	m_shader->setVec3("light.positiom", 1.2f, 1.0f, 2.0f);
	m_shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	m_shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	m_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//设置材质属性
	m_shader->setVec3("material.ambient", 1.2f, 1.0f, 2.0f);
	m_shader->setVec3("material.diffuse", 0.2f, 0.2f, 0.2f);
	m_shader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	m_shader->setFloat("material.shininess", 32.0f);
	
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);



	// 将离屏帧缓冲添加到纹理中
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, 200, 200); // 设置视口大小为 200x200
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//glClear(GL_COLOR_BUFFER_BIT);
	screen_shader_->UseProgram();
	//LogInfo("glGetError()1:{}", int(glGetError()));
	glBindVertexArray(quadvao_);
	//LogInfo("glGetError()2:{}", int(glGetError()));
	glBindTexture(GL_TEXTURE_2D, m_PickTexture);	// use the color attachment texture as the texture of the quad plane
	//LogInfo("glGetError()3:{}", int(glGetError()));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	LogInfo("glGetError()4:{}", int(glGetError()));


#endif
	

#if 1
	// 步骤二：创建拾取纹理
		//// 清除缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, m_PickFBO);
	glViewport(0, 0, m_Width, m_Height);
	LogInfo("PickModel: width: {} height: {}", m_Width, m_Height);
	// 清除缓冲
	glClearColor(0.5f, 0.3f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);



	m_shader->UseProgram();
	m_shader->setMat4("view", mvp_data_->view_);
	m_shader->setMat4("projection", mvp_data_->projection_);
	m_shader->setMat4("model", mvp_data_->model_);


	//设置光源属性
	m_shader->setVec3("viewPos", mvp_data_->view_[3]);
	m_shader->setVec3("light.positiom", 1.2f, 1.0f, 2.0f);
	m_shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	m_shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	m_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//设置材质属性
	m_shader->setVec3("material.ambient", 1.2f, 1.0f, 2.0f);
	m_shader->setVec3("material.diffuse", 0.2f, 0.2f, 0.2f);
	m_shader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	m_shader->setFloat("material.shininess", 32.0f);


	//设置顶点属性指针


	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glFinish();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

}

void ScreenRenderModel::InitBufferData()
{
#if 1
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
    glGenVertexArrays(1, &quadvao_);
    glGenBuffers(1, &quadvbo_);
    glBindVertexArray(quadvao_);
    glBindBuffer(GL_ARRAY_BUFFER, quadvbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


#endif




#if 1
	//glBindVertexArray(0);
	//步骤1：创建缓冲对象
	glGenFramebuffers(1, &m_PickFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_PickFBO);


	// 步骤二：创建拾取纹理
	glGenTextures(1, &m_PickTexture);
	glBindTexture(GL_TEXTURE_2D, m_PickTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glBindTexture(GL_TEXTURE_2D, 0);

	// 附加颜色纹理到FBO（还可以是深度、模板纹理附件）
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PickTexture, 0);




	// 创建渲染缓冲对象（深度和模板）
	glGenRenderbuffers(1, &m_PickRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_PickRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//附加渲染缓冲对象到FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_PickRBO);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LogInfo("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	}
	//// 解绑FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	std::cout << "FBO initialization completed" << std::endl;


#endif

}


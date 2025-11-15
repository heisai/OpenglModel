#include"ScreenRenderModel.h"
ScreenRenderModel::ScreenRenderModel(std::shared_ptr<Shader> shader /*= nullptr*/):
    GraphicsEngine(shader)
{
	screen_shader_ = std::make_unique<Shader>("screen_vertex.vs", "screen_fragment.fs", "GeneralModel");
	color_pick_shader_ = std::make_unique<Shader>("pick_vertex.vs", "pick_fragment.fs", "GeneralModel");
}


void ScreenRenderModel::Draw()
{
	// 将离屏帧缓冲添加到纹理中
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, 200, 200); // 设置视口大小为 200x200
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//glClear(GL_COLOR_BUFFER_BIT);
	screen_shader_->bind();
	glBindVertexArray(quadvao_);
	glBindTexture(GL_TEXTURE_2D, m_PickTexture);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);






	// 步骤二：创建拾取纹理
		//// 清除缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, m_PickFBO);
	glViewport(0, 0, m_Width, m_Height);
	// 清除缓冲
	glClearColor(0.5f, 0.3f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);



	m_shader->bind();
	//设置顶点属性指针
	glBindVertexArray(default_render_vao_);
	glDrawElements(GL_TRIANGLES, default_render_indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glFinish();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ScreenRenderModel::InitBufferData()
{
	screen_shader_->CreatProgram();
	screen_shader_->SetInt("screenTexture", 0);
	color_pick_shader_->CreatProgram();

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
}

void ScreenRenderModel::setModelData(const ModelDataInfo& datas)
{

}

void ScreenRenderModel::setScreenRenderVertexData(unsigned int vao, std::vector<unsigned int>indices)
{
	 default_render_vao_ = vao;
	 default_render_indices_ = indices;
}


bool ScreenRenderModel::colorPick(glm::mat4 model, glm::mat4 view, glm::mat4 projection, int readX, int readY,
	int objetc_id)
{
	LogInfo("========================== START PICKING =======================");
#if 1
	// 保存当前状态
	GLint currentFBO;
	glGetIntegerv(GL_FRAMEBUFFER, &currentFBO);
	GLint currentReadBuffer;
	glGetIntegerv(GL_READ_BUFFER, &currentReadBuffer);



	glBindFramebuffer(GL_FRAMEBUFFER, m_PickFBO);
	glViewport(0, 0, m_Width, m_Height);

	LogInfo("PickModel: width: {} height: {}", m_Width, m_Height);

	// 清除缓冲
	//glClearColor(0.5f, 0.3f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// 使用拾取着色器

	color_pick_shader_->bind();
	color_pick_shader_->setMat4("projection", projection);
	color_pick_shader_->setMat4("view", view);
	color_pick_shader_->setMat4("model", model);

	// 设置拾取颜色
	glm::vec3 pickColor = idToColor(objetc_id);
	LogInfo("Object ID {} \t Pick RGB:{},{},{}", objetc_id, (float)pickColor.r, (float)pickColor.g, (float)pickColor.b);
	color_pick_shader_->setVec3("pickColor", pickColor);

	glBindVertexArray(default_render_vao_);
	glDrawElements(GL_TRIANGLES, default_render_indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glFinish();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_PickFBO);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	// 读取像素
	unsigned char pixel[3] = { 0, 0, 0 };
	glReadPixels(readX, readY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

	// 转换回ID
	int pickedID = colorToId(glm::vec3(pixel[0], pixel[1], pixel[2]));

	GLint srgb;
	glGetIntegerv(GL_FRAMEBUFFER_SRGB, &srgb);
	LogInfo("Picked ID {} \t Pixel RGB:{},{},{}", pickedID, (int)pixel[0], (int)pixel[1], (int)pixel[2]);
	// === 关键修正7：正确恢复状态 ===
	glReadBuffer(currentReadBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_Width, m_Height);
	return pickedID == objetc_id;
#endif
}

glm::vec3 ScreenRenderModel::idToColor(int id)
{
	float r = ((id >> 0) & 0xFF) / 255.0f;
	float g = ((id >> 8) & 0xFF) / 255.0f;
	float b = ((id >> 16) & 0xFF) / 255.0f;
	return glm::vec3(r, g, b);
}

int ScreenRenderModel::colorToId(const glm::vec3& color)
{
	int r = int(color.r * 255.0f + 0.5f);
	int g = int(color.g * 255.0f + 0.5f);
	int b = int(color.b * 255.0f + 0.5f);
	return (r / 255 | (g << 8) / 255 | (b << 16) / 255);
}
#include"GeneralModel.h"
GeneralModel::GeneralModel(std::shared_ptr<Shader> shader /*= nullptr*/):
    GraphicsEngine(shader),
	m_RayTrack(false)
{
	// 使用拾取着色器
	m_PickShader = std::make_shared<Shader>("pick_vertex.vs", "pick_fragment.fs", "GeneralModel");
	stencil_shader_ = std::make_shared<Shader>("stencil_vertex.vs", "stencil_fragment.fs", "GeneralModel");
	screen_shader_ = std::make_shared<Shader>("fbo_vertex.vs", "fbo_fragment.fs", "GeneralModel");

}


void GeneralModel::Draw()
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

void GeneralModel::InitBufferData()
{
	



	//glEnable(GL_DEPTH_TEST);
	////glDepthFunc(GL_LESS);
	////glEnable(GL_STENCIL_TEST);
	////glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	////glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	////glEnable(GL_MULTISAMPLE);
	////glHint(GL_LINE_SMOOTH, GL_NICEST);

	// 顶点数组对象
	glGenVertexArrays(1, &m_VAO);
	//绑定VAO
	glBindVertexArray(m_VAO);

	//把顶点数组复制到缓冲中供OpenGL使用
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//glBindFramebuffer(GL_FRAMEBUFFER, 0);






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

void GeneralModel::SetLightColor(glm::vec3 lightcolor)
{
	m_LightColor = lightcolor;
}

void GeneralModel::SetObjectColor(glm::vec3 objectcolor)
{
	m_ObjectColor = objectcolor;
}

void GeneralModel::DrawStencil(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
#if 0
	
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	m_Stencilshader->UseProgram();
	float scale = 1.025f;
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	m_Stencilshader->setMat4("model", model);
	m_Stencilshader->setMat4("view", view);
	m_Stencilshader->setMat4("projection", projection);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
#endif 
}

bool GeneralModel::PickModel(glm::mat4 model, glm::mat4 view, glm::mat4 projection, int readX, int readY, 
	int objetc_id)
{

	
	return 0;
	LogInfo( "========================== START PICKING =======================");
	
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

	m_PickShader->UseProgram();
	m_PickShader->setMat4("projection", projection);
	m_PickShader->setMat4("view", view);
	m_PickShader->setMat4("model", model);

	// 设置拾取颜色
	glm::vec3 pickColor = IdToColor(objetc_id);
	LogInfo("Object ID {} \t Pick RGB:{},{},{}", objetc_id, (float)pickColor.r, (float)pickColor.g, (float)pickColor.b);
	m_PickShader->setVec3("pickColor", pickColor);
	
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glFinish();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_PickFBO);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	// 读取像素
	unsigned char pixel[3] = { 0, 0, 0 };
	glReadPixels(readX, readY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

	// 转换回ID
	int pickedID = ColorToId(glm::vec3(pixel[0], pixel[1], pixel[2]));

	GLint srgb;
	glGetIntegerv(GL_FRAMEBUFFER_SRGB, &srgb);
	LogInfo("Picked ID {} \t Pixel RGB:{},{},{}", pickedID, (int)pixel[0], (int)pixel[1], (int)pixel[2]);
	// === 关键修正7：正确恢复状态 ===
	glReadBuffer(currentReadBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_Width, m_Height);

	selected_ =  pickedID == objetc_id;
	return selected_;
}

glm::vec3 GeneralModel::IdToColor(int id)
{
	float r = ((id >> 0) & 0xFF) / 255.0f;
	float g = ((id >> 8) & 0xFF) / 255.0f;
	float b = ((id >> 16) & 0xFF) / 255.0f;
	return glm::vec3(r, g, b);
}

int GeneralModel::ColorToId(const glm::vec3& color)
{
	int r = int(color.r * 255.0f + 0.5f);
	int g = int(color.g * 255.0f + 0.5f);
	int b = int(color.b * 255.0f + 0.5f);
	return (r/255| (g << 8)/255 | (b<<16)/255);

}



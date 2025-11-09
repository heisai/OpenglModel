#include"Graphics.h"

GraphicsEngine::GraphicsEngine(std::shared_ptr<Shader> shader /*= nullptr*/,
	std::shared_ptr<Shader>m_Tshader /*= nullptr*/):
	m_shader(shader),
	m_PickShader(m_Tshader)
{
	initializeOpenGLFunctions();
	mvp_data_ = std::make_shared<MvpData>();
}
void GraphicsEngine::SetModelData(const ModelDataInfo & datas)
{
     m_Vertices = datas.m_Vertices;
    m_Indices = datas.m_Indices;
}
void GraphicsEngine::SetViewSize(int width,int height)
{
	m_Width = width;
	m_Height = height;
}
void GraphicsEngine::UseShader()
{
	glUseProgram(m_shader->ShaderPromger);

}

void GraphicsEngine::setChecked(bool flag)
{
	selected_ = flag;
}

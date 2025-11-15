#include"Graphics.h"

GraphicsEngine::GraphicsEngine(EngineType type,std::shared_ptr<Shader> shader /*= nullptr*/,
	std::shared_ptr<Shader>m_Tshader /*= nullptr*/):
	m_shader(shader),
	m_PickShader(m_Tshader)
{
	initializeOpenGLFunctions();
	mvp_data_ = std::make_shared<MvpData>();

	setModelInfo(CreatModelData().GetModelDatas(type));
}

void GraphicsEngine::setModelInfo(const ModelDataInfo& model_datas)
{
	vertices_datas = model_datas.vertices_datas;
	indices_datas = model_datas.indices_datas;
}

void GraphicsEngine::setModelData(const glm::mat4& model_)
{
	mvp_data_->model_ = model_;
}

void GraphicsEngine::setProjectionData(const glm::mat4& projection_)
{
	mvp_data_->projection_ = projection_;
}
void GraphicsEngine::setTranlstorPosition(const QVector2D& tranlstor_position_)
{
	mvp_data_->tranlstor_position_ = tranlstor_position_;
}

MvpDataPtr GraphicsEngine::getMvpData()
{
		return mvp_data_;
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

void GraphicsEngine::setViewData(const glm::mat4& view_)
{
	mvp_data_->view_ = view_;
}

#include"Graphics.h"

GraphicsEngine::GraphicsEngine(OperatorAction type,std::shared_ptr<Shader> shader /*= nullptr*/,
	std::shared_ptr<Shader>m_Tshader /*= nullptr*/):
	default_shader_(shader),
	/*m_PickShader(m_Tshader),*/
	model_type_(type)
{
	initializeOpenGLFunctions();
	mvp_data_ = std::make_shared<MvpData>();
	mesh_data_ = std::make_shared<Mesh>();
	setModelInfo(CreatModelData().GetModelDatas(type));

	//默认材质信息
	materal_.ambient_ = glm::vec3(0.0215f, 0.1745f, 0.0215f);
	materal_.diffsue_ = glm::vec3(0.07568f, 0.61424f, 0.07568f);
	materal_.specular_ = glm::vec3(0.633f, 0.727811f, 0.633f);
	materal_.shininess_ = 76.8f;
}

void GraphicsEngine::setModelInfo(const ModelDataInfo& model_datas)
{
	mesh_data_->vertices_datas = model_datas.vertices_datas;
	mesh_data_->indices_datas = model_datas.indices_datas;
	mesh_data_->model_name_ = model_datas.model_name_;
	mesh_data_->model_type_ = model_type_;
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

void GraphicsEngine::setMaterialData(const Utils::Material& material)
{
	materal_ = material;
}

MvpDataPtr GraphicsEngine::getMvpData()
{
		return mvp_data_;
}



MeshPtr GraphicsEngine::getMesh()
{
	return mesh_data_;
}

OperatorAction GraphicsEngine::getModeltype()const
{
	return model_type_;
}

QString GraphicsEngine::getModelName() 
{
	if (mesh_data_)
	{
		return mesh_data_->model_name_;
	}
	return "";
}

void GraphicsEngine::setModelName(const QString& model_name)
{
	mesh_data_->model_name_ = model_name;
}


bool GraphicsEngine::getCheck() const
{
	return selected_;
}

std::shared_ptr<Shader> GraphicsEngine::getDefaultShader()
{
	return default_shader_;
}

void GraphicsEngine::SetViewSize(int width,int height)
{
	m_Width = width;
	m_Height = height;
}
void GraphicsEngine::UseShader()
{
	glUseProgram(default_shader_->ShaderPromger);

}

void GraphicsEngine::setChecked(bool flag)
{
	selected_ = flag;
}

void GraphicsEngine::setViewData(const glm::mat4& view_)
{
	mvp_data_->view_ = view_;
}

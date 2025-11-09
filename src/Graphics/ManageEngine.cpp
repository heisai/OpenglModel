#include "ManageEngine.h"

ManageEngine::ManageEngine()
{
}

void ManageEngine::setViewSize(int width, int height)
{
    width_ = width;
    height_ = height;
    std::for_each(map_graphic_.begin(), map_graphic_.end(), [width, height](auto pair) {pair.second->SetViewSize(width, height); });

}

void ManageEngine::createModel(EngineType type)
{
    switch (type)
    {
    case EM_GRIDENGINE:
        createGridEngine();
        break;
    case EM_CUBEENGINE:
        createCubeEngine();
        break;
    case EM_TOURSENGINE:
        createToursEngine();
        break;
    case EM_MODELENGINE:
        createLoadModelEngine();
        break;
    case EM_CYLINDERENGINE:
        createCylinderEngine();
        break;
    default:
        break;
    }
}

void ManageEngine::createGridEngine()
{
 /*   ShaderPtr shader = std::make_shared<Shader>("123.txt", "456.txt", "Rectangle");
    GraphicsEnginePtr graphics = std::make_shared<RectangleEngine>(shader);
    addEngine(generateUuid(), graphics, shader);*/
}

void ManageEngine::createCubeEngine()
{

}

void ManageEngine::createToursEngine()
{
    ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs", "GeneralModel");
    auto vertices_data = std::make_unique<CreatModelData>();
    GraphicsEnginePtr basic_light_engine = std::make_shared<GeneralModel>(light_shader);
    basic_light_engine->SetModelData(vertices_data->GetModelDatas(EM_TOURSENGINE));
    basic_light_engine->SetViewSize(width_, height_);
    addEngine(generateUuid(), basic_light_engine, light_shader);
}

void ManageEngine::createCylinderEngine()
{
    ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs", "GeneralModel");
    auto vertices_data = std::make_unique<CreatModelData>();
    GraphicsEnginePtr basic_light_engine = std::make_shared<GeneralModel>(light_shader);
    basic_light_engine->SetModelData(vertices_data->GetModelDatas(EM_CYLINDERENGINE));
    basic_light_engine->SetViewSize(width_, height_);
    addEngine(generateUuid(), basic_light_engine, light_shader);
}

void ManageEngine::createLoadModelEngine()
{
    //ShaderPtr model_shader = std::make_shared<Shader>("123.vert", "456.frag", "LoadModel");
    //GraphicsEnginePtr load_model_engine = std::make_shared<LoadModelEngine>(model_shader);
    //addEngine(generateUuid(), load_model_engine, model_shader);
}

void ManageEngine::createMixEngine()
{
    //ShaderPtr model_shader = std::make_shared<Shader>("123.vert", "456.frag", "MixEngine");
    //GraphicsEnginePtr mix_engine = std::make_shared<MixEngine>(model_shader);
    //addEngine(generateUuid(), mix_engine, model_shader);
}

void ManageEngine::addEngine(const QString& uuid, const GraphicsEnginePtr& graphics, const ShaderPtr& shader)
{
    if (map_graphic_.find(uuid) == map_graphic_.end())
    {
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        graphics->mvp_data_->view_ = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
        graphics->mvp_data_->model_ = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        graphics->mvp_data_->projection_ = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
        map_graphic_.emplace(uuid, graphics);
    }

    for (const auto& pair_value : map_graphic_)
    {
        LogInfo("EngineType:{}", pair_value.first.toStdString());
    }
}

QString ManageEngine::generateUuid()
{
    QUuid id = QUuid::createUuid();
    QString str_id = id.toString();
    str_id.remove("{").remove("}").remove("-");
    return str_id;
}

void ManageEngine::removeEngine(EngineType /*type*/)
{
    // 保留实现占位，按需实现
}

void ManageEngine::checkBoxTypeSlot(const QString& type, bool check)
{
    if (map_graphic_.count(type))
    {
        /*PhongModelEngine* engine = dynamic_cast<PhongModelEngine*>(map_graphic_[type].get());
        if (engine)
        {
            engine->m_RayTrack = check;
        }*/
    }
}

void ManageEngine::initializeGl()
{
    for (const auto& pair : map_graphic_)
    {
        if (pair.second && pair.second->m_shader)
            pair.second->m_shader->CreatProgram();
        if (pair.second && pair.second->m_PickShader)
            pair.second->m_PickShader->CreatProgram();
        if (pair.second && pair.second->stencil_shader_)
        {
            pair.second->stencil_shader_->CreatProgram();
        }
		if (pair.second && pair.second->screen_shader_)
		{
			pair.second->screen_shader_->CreatProgram();
            pair.second->screen_shader_->SetInt("screenTexture", 0);
		}
        if (pair.second)
            pair.second->InitBufferData();
    }
}



void ManageEngine::setEngineScaleAndTranslate(const QString& uuid, const glm::vec3& scale, const glm::vec3& translate, const glm::mat4& model_old)
{
    int index = 0;
    for (auto& onepair : map_graphic_)
    {
        if (onepair.second->selected_)
        {
            QString key = onepair.first;
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);

            projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
            view = glm::translate(view, glm::vec3(translate.x, translate.y, translate.z));
            model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));
            map_graphic_[key]->mvp_data_->view_ = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            map_graphic_[key]->mvp_data_->model_ = model * model_old;
            map_graphic_[key]->mvp_data_->projection_ = projection;
            map_graphic_[key]->mvp_data_->tranlstor_position_ = QVector2D(translate.x, -translate.y);
        }
        
    }
}

void ManageEngine::paintGl()
{
    if (map_graphic_.empty()) return;

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    for (const auto& pair : map_graphic_)
    {
        if (!pair.second) continue;
        if (pair.second->m_shader) pair.second->m_shader->UseProgram();
        pair.second->Draw();
    }
}

MvpDataPtr ManageEngine::pickModel(int xpos, int ypos)
{
    MvpDataPtr mvp_data = nullptr;
    if (map_graphic_.empty())
    {
        mvp_data;
    }
    //清空物体选中状态
    std::for_each(map_graphic_.begin(), map_graphic_.end(), [](auto pair) {pair.second->setChecked(false); });


    int object_id = 1;
    for (const auto& pair : map_graphic_)
    {
        QString model_id = pair.first;
        MvpDataPtr mvp = pair.second->mvp_data_;
        bool selected = pair.second->PickModel(mvp->model_, mvp->view_, mvp->projection_, xpos, ypos, object_id);
        if (selected)
        {
            LogInfo("【Selected Model UUID:{}】 ColorID: {}", model_id.toStdString(), object_id);
            mvp_data = mvp;
            break;
        }
        ++object_id;
    }
    return mvp_data;
}

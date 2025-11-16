#include "ManageEngine.h"

ManageEngine::ManageEngine():
    screen_render_model_(nullptr)
{

	// 注册引擎创建函数
    map_graphicengine_createfunc_.insert({ EM_CUBEENGINE, std::bind(&ManageEngine::createCubeEngine, this) });
    map_graphicengine_createfunc_.insert({ EM_TOURSENGINE, std::bind(&ManageEngine::createToursEngine, this) });
    map_graphicengine_createfunc_.insert({ EM_CYLINDERENGINE, std::bind(&ManageEngine::createCylinderEngine, this) });
    map_graphicengine_createfunc_.insert({ EM_SKYBOXENGINE, std::bind(&ManageEngine::createSkyBoxEngine, this) });


}

void ManageEngine::setViewSize(int width, int height)
{
    width_ = width;
    height_ = height;
    std::for_each(list_graphic_.begin(), list_graphic_.end(), [width, height](auto graphic) {graphic ->SetViewSize(width, height); });

	if (screen_render_model_)
	{
        screen_render_model_->SetViewSize(width, height);
	}
}

void ManageEngine::createModel(EngineType type)
{

   auto iter = map_graphicengine_createfunc_.find(type);
   if (iter != map_graphicengine_createfunc_.end())
   {
       GraphicsEnginePtr ptr = iter->second();
       addEngine(ptr);
   }
   if (screen_render_model_ == nullptr)
   {
	   screen_render_model_ = std::make_unique<ScreenRenderModel>(EM_MIXENGINE);
	   screen_render_model_->SetViewSize(width_, height_);
   }
}

void ManageEngine::removeModel(EngineType type)
{
    if (type == EM_SKYBOXENGINE)
    {
        list_graphic_.remove_if([type](auto grahic) {   return grahic->getModeltype() == type; });
    }
    else
    {
        list_graphic_.remove_if([](auto grahic) {   return grahic->getCheck() == true; });
    }
 
    
}

void ManageEngine::createGridEngine()
{
 /*   ShaderPtr shader = std::make_shared<Shader>("123.txt", "456.txt", "Rectangle");
    GraphicsEnginePtr graphics = std::make_shared<RectangleEngine>(shader);
    addEngine(generateUuid(), graphics, shader);*/
}

GraphicsEnginePtr ManageEngine::createCubeEngine()
{
	//ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs", "CubeMapsModel");
	//GraphicsEnginePtr basic_light_engine = std::make_shared<CubeMapsModel>(light_shader);
	//basic_light_engine->SetViewSize(width_, height_);
	//addEngine(generateUuid(), basic_light_engine, light_shader);

	ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs", "GeneralModel");
	GraphicsEnginePtr basic_light_engine = std::make_shared<GeneralModel>(EM_CUBEENGINE, light_shader);
	basic_light_engine->SetViewSize(width_, height_);
	return basic_light_engine;

}

GraphicsEnginePtr ManageEngine::createToursEngine()
{
    ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs", "GeneralModel");
    GraphicsEnginePtr basic_light_engine = std::make_shared<GeneralModel>(EM_TOURSENGINE,light_shader);
    basic_light_engine->SetViewSize(width_, height_);
    return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createCylinderEngine()
{
    ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs", "GeneralModel");
    GraphicsEnginePtr basic_light_engine = std::make_shared<GeneralModel>(EM_CYLINDERENGINE,light_shader);
    basic_light_engine->SetViewSize(width_, height_);
    return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createSkyBoxEngine()
{
	ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs", "CubeMapsModel");
	GraphicsEnginePtr basic_light_engine = std::make_shared<CubeMapsModel>(EM_SKYBOXENGINE, light_shader);
	basic_light_engine->SetViewSize(width_, height_);
	return basic_light_engine;
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
void ManageEngine::addEngine(const GraphicsEnginePtr& graphics)
{
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		graphics->setViewData(glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f)));
		graphics->setModelData(glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
		graphics->setProjectionData(glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f));
        if (graphics->getMesh()->model_type_ == EM_SKYBOXENGINE)
        {
            list_graphic_.push_front(graphics);
        }
        else
        {
            list_graphic_.push_back(graphics);
        }
    

	for (const auto& graphic : list_graphic_)
	{
		LogInfo("EngineType:{} ", int(graphic->getMesh()->model_type_));
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


void ManageEngine::initializeGl()
{

    for (const auto& graphic_ : list_graphic_)
	{
            graphic_->InitBufferData();
    }
	if (screen_render_model_ )
	{
        screen_render_model_->InitBufferData();
	}
}



void ManageEngine::setEngineScaleAndTranslate(const QString& uuid, const glm::vec3& scale, const glm::vec3& translate, const glm::mat4& model_old)
{
    int index = 0;
    for (auto& graphic_ : list_graphic_)
    {
        if (graphic_->getCheck())
        {
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);

            projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
            view = glm::translate(view, glm::vec3(translate.x, translate.y, translate.z));
            model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));
            graphic_->setViewData(glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)));
            graphic_->setModelData(model * model_old);
            graphic_->setProjectionData(projection);
            graphic_->setTranlstorPosition(QVector2D(translate.x, -translate.y));
        }
        
    }
}

void ManageEngine::paintGl()
{
    if (list_graphic_.empty()) return;

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    for (const auto& graphic_ : list_graphic_)
    {
        if (graphic_->m_shader)
        {
            graphic_->m_shader->bind();
        }
        graphic_->Draw();
    }
}

MvpDataPtr ManageEngine::pickModel(int xpos, int ypos)
{
    MvpDataPtr mvp_data = nullptr;
    if (list_graphic_.empty())
    {
        mvp_data;
    }
    //清空物体选中状态
    std::for_each(list_graphic_.begin(), list_graphic_.end(), [](auto graphic_) {graphic_->setChecked(false); });


    int object_id = 1;
    for (auto& graphic_ : list_graphic_)
    {
        MvpDataPtr mvp = graphic_->getMvpData();
        //bool selected = pair.second->colorPick(mvp->model_, mvp->view_, mvp->projection_, xpos, ypos, object_id);


        MeshPtr mesh = graphic_->getMesh();
        screen_render_model_->setScreenRenderVertexData(mesh->vao_, mesh->indices_datas);
        bool selected  = screen_render_model_->colorPick(mvp->model_, mvp->view_, mvp->projection_, xpos, ypos, object_id);
        if (selected)
        {
            graphic_->setChecked(selected);
            LogInfo("【Selected 】 ColorID: {}", object_id);
            mvp_data = mvp;
            break;
        }
        ++object_id;
    }
    return mvp_data;
}

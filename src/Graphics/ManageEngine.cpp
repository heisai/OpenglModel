#include "ManageEngine.h"

ManageEngine::ManageEngine():
    screen_render_model_(nullptr)
{

	// 注册引擎创建函数
    map_graphicengine_createfunc_.insert({ OperatorAction::CreatCube, std::bind(&ManageEngine::createCubeEngine, this) });
    map_graphicengine_createfunc_.insert({ OperatorAction::CreatTourse, std::bind(&ManageEngine::createToursEngine, this) });
    map_graphicengine_createfunc_.insert({ OperatorAction::CreatCyliner, std::bind(&ManageEngine::createCylinderEngine, this) });
    map_graphicengine_createfunc_.insert({ OperatorAction::ClearSkyBox, std::bind(&ManageEngine::createSkyBoxEngine, this) });
    map_graphicengine_createfunc_.insert({ OperatorAction::CreatGrid, std::bind(&ManageEngine::createGridEngine, this) });

    map_graphicengine_createfunc_.insert({ OperatorAction::RenderInversion, std::bind(&ManageEngine::createInversionRender, this) });   //反向
    map_graphicengine_createfunc_.insert({ OperatorAction::RenderGrayscale, std::bind(&ManageEngine::createGrayscaleRender, this) });   //灰度
    map_graphicengine_createfunc_.insert({ OperatorAction::RenderSharpen, std::bind(&ManageEngine::createSharpenRender, this) });   //锐化
    map_graphicengine_createfunc_.insert({ OperatorAction::RenderBlur, std::bind(&ManageEngine::createBlurRender, this) });   //模糊
    map_graphicengine_createfunc_.insert({ OperatorAction::RenderDetection, std::bind(&ManageEngine::createDetectionRender, this) });   //检测

   
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

void ManageEngine::createModel(OperatorAction type)
{
    //判断包装器中是否包含该函数
   auto iter = map_graphicengine_createfunc_.find(type);
   if (iter != map_graphicengine_createfunc_.end())
   {
       GraphicsEnginePtr ptr = iter->second();
       addEngine(ptr);
   }
   if (screen_render_model_ == nullptr)
   {
	   screen_render_model_ = std::make_unique<ScreenRenderModel>(OperatorAction::CreatTourse);
	   screen_render_model_->SetViewSize(width_, height_);
   }
}

void ManageEngine::removeModel(OperatorAction type)
{
    if (type == OperatorAction::ClearSkyBox)
    {
        list_graphic_.remove_if([type](auto grahic) {   return grahic->getModeltype() == type; });
    }
    else
    {
        list_graphic_.remove_if([](auto grahic) {   return grahic->getCheck() == true; });
    }
 
    
}

GraphicsEnginePtr ManageEngine::createGridEngine()
{
	/*ShaderPtr light_shader = std::make_shared<Shader>("grid_vertex.vert", "grid_fragment.frag", "GeneralModel");
	GraphicsEnginePtr basic_light_engine = std::make_shared<GridModel>(OperatorAction::CreatGrid, light_shader);
	basic_light_engine->SetViewSize(width_, height_);
	return basic_light_engine;*/


	ShaderPtr light_shader  = std::make_shared<Shader>("axis_vertex.vert", "axis_fragment.frag","axis_geometry.vert", "GeneralModel");
	GraphicsEnginePtr basic_light_engine = std::make_shared<AxisModel>(OperatorAction::CreatAxis, light_shader);
	basic_light_engine->SetViewSize(width_, height_);
	return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createCubeEngine()
{
	ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs", "GeneralModel");
	GraphicsEnginePtr basic_light_engine = std::make_shared<GeneralModel>(OperatorAction::CreatCube, light_shader);
	basic_light_engine->SetViewSize(width_, height_);
	return basic_light_engine;

}

GraphicsEnginePtr ManageEngine::createToursEngine()
{
    ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs", "GeneralModel");
    GraphicsEnginePtr basic_light_engine = std::make_shared<GeneralModel>(OperatorAction::CreatTourse,light_shader);
    basic_light_engine->SetViewSize(width_, height_);
    return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createCylinderEngine()
{
    ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs", "GeneralModel");
    GraphicsEnginePtr basic_light_engine = std::make_shared<GeneralModel>(OperatorAction::CreatCyliner,light_shader);
    basic_light_engine->SetViewSize(width_, height_);
    return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createSkyBoxEngine()
{
	ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vs", "fragment_shader.fs", "CubeMapsModel");
	GraphicsEnginePtr basic_light_engine = std::make_shared<CubeMapsModel>(OperatorAction::ClearSkyBox, light_shader);
	basic_light_engine->SetViewSize(width_, height_);
	return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createInversionRender()
{
    GraphicsEnginePtr basic_light_engine = nullptr;
	if (screen_render_model_)
	{
		screen_render_model_->setRenderType(1);
	}
    return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createGrayscaleRender()
{
	GraphicsEnginePtr basic_light_engine = nullptr;
	if (screen_render_model_)
	{
		screen_render_model_->setRenderType(2);
	}
	return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createSharpenRender()
{
	GraphicsEnginePtr basic_light_engine = nullptr;
	if (screen_render_model_)
	{
		screen_render_model_->setRenderType(3);
	}
	return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createBlurRender()
{
	GraphicsEnginePtr basic_light_engine = nullptr;
	if (screen_render_model_)
	{
		screen_render_model_->setRenderType(4);
	}
	return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createDetectionRender()
{
	GraphicsEnginePtr basic_light_engine = nullptr;
	if (screen_render_model_)
	{
		screen_render_model_->setRenderType(5);
	}
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
	if (graphics == nullptr)
	{
		return;
	}
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	graphics->setViewData(glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f)));
	graphics->setModelData(glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
	graphics->setProjectionData(glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f));
	if (graphics->getMesh()->model_type_ == OperatorAction::ClearSkyBox)
	{
		list_graphic_.push_front(graphics);
	}
	else
	{
		list_graphic_.push_back(graphics);
	}


	for (const auto& graphic : list_graphic_)
	{
		LogInfo("OperatorAction:{} ", int(graphic->getMesh()->model_type_));
	}
}

QString ManageEngine::generateUuid()
{
    QUuid id = QUuid::createUuid();
    QString str_id = id.toString();
    str_id.remove("{").remove("}").remove("-");
    return str_id;
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

	auto iter = std::find_if(list_graphic_.begin(), list_graphic_.end(), [](GraphicsEnginePtr graphic_) {
		return (graphic_->getCheck() == true);
		});

	if (iter != list_graphic_.end())
	{
		SetEngineScaleAndTranslate(*iter, scale, translate, model_old);
		iter = std::find_if(list_graphic_.begin(), list_graphic_.end(), [](GraphicsEnginePtr graphic_) {
			return (graphic_->getModeltype() == OperatorAction::CreatAxis);
			});
		if (iter != list_graphic_.end())
		{
			SetEngineScaleAndTranslate(*iter, scale, translate, model_old);
		}
	}

}
void ManageEngine::SetEngineScaleAndTranslate(const GraphicsEnginePtr& engine_ptr, const glm::vec3& scale, const glm::vec3& translate, const glm::mat4& model_old)
{
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
	view = glm::translate(view, glm::vec3(translate.x, translate.y, translate.z));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));
	engine_ptr->setViewData(glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)));
	engine_ptr->setModelData(model * model_old);
	engine_ptr->setProjectionData(projection);
	engine_ptr->setTranlstorPosition(QVector2D(translate.x, -translate.y));
}

void ManageEngine::paintGl()
{


    if (list_graphic_.empty()) return;

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    for (const auto& graphic_ : list_graphic_)
    {
        if (graphic_->getDefaultShader())
        {
            graphic_->getDefaultShader()->bind();
            graphic_->Draw();
        }
    }
    //如果模型是空，不进行离屏渲染
    if (list_graphic_.empty())
    {
        return;
    }

	if (screen_render_model_)
	{
		screen_render_model_->drawTexture();
		for (const auto& graphic_ : list_graphic_)
		{
				MeshPtr mesh = graphic_->getMesh();
				screen_render_model_->setScreenRenderVertexData(mesh->vao_, mesh->indices_datas, graphic_->getDefaultShader());
				screen_render_model_->Draw();
		}
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
    std::map<float, GraphicsEnginePtr>map_engineptr;
    for (auto& graphic_ : list_graphic_)
    {
        MvpDataPtr mvp = graphic_->getMvpData();
        MeshPtr mesh = graphic_->getMesh();
        screen_render_model_->setScreenRenderVertexData(mesh->vao_, mesh->indices_datas);
        auto[selected,depth] = screen_render_model_->colorPick(mvp->model_, mvp->view_, mvp->projection_, xpos, ypos, object_id);
        if (selected)
        {
            LogInfo("【Selected 】 ColorID: {} \n Depth: {}", object_id, depth);
            map_engineptr[depth] = graphic_;
        }
        ++object_id;
    }
    if (!map_engineptr.empty())
    {
        map_engineptr.begin()->second->setChecked(true);
        return map_engineptr.begin()->second->getMvpData();
    }
    return mvp_data;
}

#include "ManageEngine.h"
#include <cmath>

namespace
{
// 检查向量各分量是否有限，拒绝 NaN 和无穷值进入场景状态。
bool finiteVector(const glm::vec3 &value)
{
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}
} // namespace

ManageEngine::ManageEngine() : screen_render_model_(nullptr)
{
    registerEngineModel();
}

void ManageEngine::setViewSize(int width, int height)
{
    width_ = width;
    height_ = height;
    if (width > 0 && height > 0)
    {
        const auto projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
        for (const auto &graphic : list_graphic_)
        {
            graphic->SetViewSize(width, height);
            graphic->setProjectionData(projection);
        }
    }

    if (screen_render_model_)
    {
        screen_render_model_->SetViewSize(width, height);
    }
}

QString ManageEngine::createModel(OperatorAction type)
{
    QString model_uuid = "";
    // 判断包装器中是否包含该函数
    auto iter = map_graphicengine_createfunc_.find(type);
    if (iter != map_graphicengine_createfunc_.end())
    {
        GraphicsEnginePtr ptr = iter->second();
        if (!ptr)
        {
            return model_uuid; // 渲染类型不创建模型
        }
        addEngine(ptr);
        model_uuid = ptr->getModelName();
    }
    if (screen_render_model_ == nullptr)
    {
        screen_render_model_ = std::make_unique<ScreenRenderModel>(OperatorAction::CreatTourse);
        screen_render_model_->SetViewSize(width_, height_);
    }
    if (shadow_map_pass_ == nullptr)
    {
		shadow_map_pass_ = std::make_unique<ShadowMapPass>(OperatorAction::RenderShadow);
		shadow_map_pass_->SetViewSize(width_, height_);
    }
    return model_uuid;
}

QString ManageEngine::removeModel(OperatorAction type)
{

    QString model_uuid = "";
    if (type == OperatorAction::ClearBlasting) // 删除爆破功能
    {
        auto iter = std::find_if(list_graphic_.begin(), list_graphic_.end(),
                                 [](GraphicsEnginePtr graphic_) 
            { 
                return (graphic_->getCheck() == true);
            });

        if (iter != list_graphic_.end())
        {
            (*iter)->getMvpData()->blasting_flag_ = false;
        }
        return "";
    }
    auto iter = std::find_if(list_graphic_.begin(), list_graphic_.end(),
        [type](const GraphicsEnginePtr& graphic)
        {
            if (type == OperatorAction::ClearSkyBox)
            {
                return graphic->getModeltype() == OperatorAction::ClearSkyBox;
            }
            return graphic->getCheck();
        });

    if (iter == list_graphic_.end())
    {
        return {};
    }
    const QString removedName = (*iter)->getModelName();
    list_graphic_.erase(iter);
    return removedName;
  
}

void ManageEngine::registerEngineModel()
{

    // 注册引擎创建函数
    // 立方体
    map_graphicengine_createfunc_.insert({OperatorAction::CreatCube, std::bind(&ManageEngine::createCubeEngine, this)});
    // 圆环体
    map_graphicengine_createfunc_.insert(
        {OperatorAction::CreatTourse, std::bind(&ManageEngine::createToursEngine, this)});
    // 圆柱体
    map_graphicengine_createfunc_.insert(
        {OperatorAction::CreatCyliner, std::bind(&ManageEngine::createCylinderEngine, this)});
    // 天空盒
    map_graphicengine_createfunc_.insert(
        {OperatorAction::ClearSkyBox, std::bind(&ManageEngine::createSkyBoxEngine, this)});
    // 网格
    map_graphicengine_createfunc_.insert({OperatorAction::CreatGrid, std::bind(&ManageEngine::createGridEngine, this)});
    // 行星
    map_graphicengine_createfunc_.insert(
        {OperatorAction::CreatPlanet, std::bind(&ManageEngine::createPlanteEngine, this)});
    // 反向
    map_graphicengine_createfunc_.insert(
        {OperatorAction::RenderInversion, std::bind(&ManageEngine::createInversionRender, this)});
    // 灰度
    map_graphicengine_createfunc_.insert(
        {OperatorAction::RenderGrayscale, std::bind(&ManageEngine::createGrayscaleRender, this)});

    map_graphicengine_createfunc_.insert(
        {OperatorAction::RenderSharpen, std::bind(&ManageEngine::createSharpenRender, this)}); // 锐化

    map_graphicengine_createfunc_.insert(
        {OperatorAction::RenderBlur, std::bind(&ManageEngine::createBlurRender, this)}); // 模糊

    map_graphicengine_createfunc_.insert(
        {OperatorAction::RenderDetection, std::bind(&ManageEngine::createDetectionRender, this)}); // 检测

    map_graphicengine_createfunc_.insert(
        {OperatorAction::RenderBlasting, std::bind(&ManageEngine::createBlastingRender, this)}); // 爆破

    map_graphicengine_createfunc_.insert(
        {OperatorAction::ClearRender, std::bind(&ManageEngine::clearRender, this)}); // 清除渲染

    // 坐标轴
    map_graphicengine_createfunc_.insert({OperatorAction::CreatAxis, std::bind(&ManageEngine::createAxisEngine, this)});
    // 灯光
    map_graphicengine_createfunc_.insert(
        {OperatorAction::CreatLight, std::bind(&ManageEngine::createLightEngine, this)});
    //深度测试
	map_graphicengine_createfunc_.insert(
		{ OperatorAction::RenderShadow, std::bind(&ManageEngine::createShadowMapPass, this) });
}

GraphicsEnginePtr ManageEngine::createAxisEngine()
{
    ShaderPtr light_shader =
        std::make_shared<Shader>("axis_vertex.vert", "axis_fragment.frag", "axis_geometry.geom", "GeneralModel");
    GraphicsEnginePtr basic_light_engine = std::make_shared<AxisModel>(OperatorAction::CreatAxis, light_shader);
    basic_light_engine->SetViewSize(width_, height_);
    return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createGridEngine()
{
    ShaderPtr light_shader = std::make_shared<Shader>("grid_vertex.vert", "scene_fragment.frag", "GridModel");
    GraphicsEnginePtr basic_light_engine = std::make_shared<GridModel>(OperatorAction::CreatGrid, light_shader);
    basic_light_engine->SetViewSize(width_, height_);
    return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createCubeEngine()
{
    ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vert", "scene_fragment.frag", "ShaderModel");
    GraphicsEnginePtr basic_light_engine = std::make_shared<ShaderModel>(OperatorAction::CreatCube, light_shader);
    basic_light_engine->SetViewSize(width_, height_);
    return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createLightEngine()
{
    ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vert", "fragment_shader.frag", "LightModel");
    GraphicsEnginePtr light_engine = std::make_shared<LightModel>(OperatorAction::CreatLight, light_shader);
    light_engine->SetViewSize(width_, height_);
    return light_engine;
}

GraphicsEnginePtr ManageEngine::createToursEngine()
{
    ////"geometry_shader.geom"
    // ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vert", "fragment_shader.frag",
    // "geometry_shader.geom", "GeneralModel");
    //   GraphicsEnginePtr basic_light_engine =
    //   std::make_shared<GeneralModel>(OperatorAction::CreatTourse,light_shader);
    //   basic_light_engine->SetViewSize(width_, height_);
    //   return basic_light_engine;

    ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vert", "scene_fragment.frag", "ShaderModel");
    GraphicsEnginePtr basic_light_engine = std::make_shared<ShaderModel>(OperatorAction::CreatTourse, light_shader);
    basic_light_engine->SetViewSize(width_, height_);
    return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createCylinderEngine()
{
    ShaderPtr light_shader = std::make_shared<Shader>("vertex_shader.vert", "..\\common\\scene_fragment.frag",
                                                      "geometry_shader.geom", "GeneralModel");
    GraphicsEnginePtr basic_light_engine = std::make_shared<GeneralModel>(OperatorAction::CreatCyliner, light_shader);
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

GraphicsEnginePtr ManageEngine::createPlanteEngine()
{
    ShaderPtr light_shader =
        std::make_shared<Shader>("vertex_shader.vert", "..\\common\\scene_fragment.frag", "PlanteModel");
    GraphicsEnginePtr basic_light_engine = std::make_shared<PlanetModel>(OperatorAction::CreatTourse, light_shader);
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

GraphicsEnginePtr ManageEngine::createBlastingRender()
{
    auto iter = std::find_if(list_graphic_.begin(), list_graphic_.end(),
                             [](GraphicsEnginePtr graphic_) { return (graphic_->getCheck() == true); });

    if (iter != list_graphic_.end())
    {
        (*iter)->getMvpData()->blasting_flag_ = true;
    }

    return nullptr;
}

GraphicsEnginePtr ManageEngine::clearRender()
{
    GraphicsEnginePtr basic_light_engine = nullptr;
    if (screen_render_model_)
    {
        screen_render_model_->setRenderType(0);
    }
    return basic_light_engine;
}

GraphicsEnginePtr ManageEngine::createShadowMapPass()
{
    ShaderPtr depth_shader =
        std::make_shared<Shader>("shadow_depth.vert", "shadow_depth.frag", "ShadowMapPass");
    GraphicsEnginePtr engine = std::make_shared<ShadowMapPass>(OperatorAction::RenderShadow, depth_shader);
    engine->SetViewSize(width_, height_);
    return engine;
}

void ManageEngine::createLoadModelEngine()
{
    // ShaderPtr model_shader = std::make_shared<Shader>("123.vert", "456.frag", "LoadModel");
    // GraphicsEnginePtr load_model_engine = std::make_shared<LoadModelEngine>(model_shader);
    // addEngine(generateUuid(), load_model_engine, model_shader);
}

void ManageEngine::createMixEngine()
{
    // ShaderPtr model_shader = std::make_shared<Shader>("123.vert", "456.frag", "MixEngine");
    // GraphicsEnginePtr mix_engine = std::make_shared<MixEngine>(model_shader);
    // addEngine(generateUuid(), mix_engine, model_shader);
}
void ManageEngine::showAxisModel(bool flag)
{
    auto iter = std::find_if(list_graphic_.begin(), list_graphic_.end(), [flag](GraphicsEnginePtr graphic_)
                             { return graphic_->getModeltype() == OperatorAction::CreatAxis; });
    if (iter != list_graphic_.end())
    {
        (*iter)->setShown(flag);
    }
}
void ManageEngine::addEngine(const GraphicsEnginePtr &graphics)
{
    if (graphics == nullptr)
    {
        return;
    }
    auto data = graphics->getMvpData();
    graphics->setViewData(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)));
    if (graphics->getModeltype() != OperatorAction::CreatGrid &&
        graphics->getModeltype() != OperatorAction::CreatAxis && graphics->getModeltype() != OperatorAction::CreatLight)
    {
        data->rotation_ =
            glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
    }
    if (graphics->getModeltype() == OperatorAction::CreatLight)
    {
        data->position_ = light_position_;
    }
    data->rebuildModel();
    graphics->setProjectionData(glm::perspective(
        glm::radians(45.0f), height_ > 0 ? float(width_) / float(height_) : 4.0f / 3.0f, 0.1f, 100.0f));
    if (graphics->getModeltype() == OperatorAction::ClearSkyBox)
    {
        list_graphic_.push_front(graphics);
    }
    else
    {
        list_graphic_.push_back(graphics);
    }

    for (const auto &graphic : list_graphic_)
    {
        LogInfo("OperatorAction:{} ", int(graphic->getModeltype()));
    }

    QString model_name = graphics->getModelName();
    generateUuid(model_name);
    graphics->setModelName(model_name);
}

void ManageEngine::generateUuid(QString &model_name)
{
    auto iter = map_model_id_.find(model_name);
    if (iter != map_model_id_.end())
    {
        map_model_id_[model_name]++;
        model_name = model_name + "." + QString("%1").arg(map_model_id_[model_name], 3, 10, '0');
    }
    else
    {
        map_model_id_[model_name] = 0;
        model_name = model_name + "." + QString("%1").arg(map_model_id_[model_name], 3, 10, '0');
    }
}
void ManageEngine::initializeGl()
{

    for (const auto &graphic_ : list_graphic_)
    {
        graphic_->InitBufferData();
    }
    if (screen_render_model_)
    {
        screen_render_model_->InitBufferData();
    }
    if (shadow_map_pass_)
    {
        shadow_map_pass_->InitBufferData();
    }
}

void ManageEngine::syncLight()
{
    if (light_config_.lights.empty())
    {
        return;
    }
    auto &p = light_config_.lights.front().parameters;
    if (auto *d = std::get_if<Lighting::DirectionalLight>(&p))
    {
        d->direction = light_direction_;
    }
    if (auto *point = std::get_if<Lighting::PointLight>(&p))
    {
        point->position = light_position_;
        point->attenuation = light_attenuation_;
    }
    if (auto *spot = std::get_if<Lighting::SpotLight>(&p))
    {
        light_spot_.position = light_position_;
        light_spot_.direction = light_direction_;
        light_spot_.attenuation = light_attenuation_;
        *spot = light_spot_;
    }
    for (const auto &graphic : list_graphic_)
    {
        if (graphic->getModeltype() == OperatorAction::CreatLight)
        {
            auto data = graphic->getMvpData();
            data->position_ = light_position_;
            data->rebuildModel();
            if (graphic->getCheck())
            {
                for (const auto &axis : list_graphic_)
                {
                    if (axis->getModeltype() == OperatorAction::CreatAxis)
                    {
                        axis->setModelData(data->model_);
                    }
                }
            }
        }
    }
    emit lightChanged();
}

void ManageEngine::setPrimaryLightValues(const glm::vec3 &position, const glm::vec3 &direction, float inner,
                                         float outer)
{
    const float directionLengthSquared = glm::dot(direction, direction);
    if (!finiteVector(position) || !finiteVector(direction) || !std::isfinite(directionLengthSquared) ||
        directionLengthSquared < 1e-8f || !std::isfinite(inner) || !std::isfinite(outer) || inner < 0.0f ||
        outer >= 90.0f || inner >= outer)
    {
        qWarning() << "Invalid light position, direction or cone angles";
        return;
    }
    light_position_ = position;
    light_direction_ = direction;
    light_spot_.innerAngleDegrees = inner;
    light_spot_.outerAngleDegrees = outer;
    syncLight();
}

void ManageEngine::updateSelectedTransform(const glm::vec3 &position, const glm::mat4 &rotation, const glm::vec3 &scale)
{
    bool finiteRotation = true;
    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            finiteRotation = finiteRotation && std::isfinite(rotation[column][row]);
        }
    }
    if (!finiteVector(position) || !finiteVector(scale) || !finiteRotation || scale.x <= 0.0f || scale.y <= 0.0f ||
        scale.z <= 0.0f)
    {
        qWarning() << "Invalid object transform";
        return;
    }
    auto it = std::find_if(list_graphic_.begin(), list_graphic_.end(), [](const auto &g) { return g->getCheck(); });
    if (it == list_graphic_.end())
    {
        return;
    }
    auto data = (*it)->getMvpData();
    if ((*it)->getModeltype() == OperatorAction::CreatLight)
    {
        // Translation does not steer the light; only the rotation delta changes direction.
        light_direction_ = glm::vec3(rotation * glm::inverse(data->rotation_) * glm::vec4(light_direction_, 0.0f));
        light_position_ = position;
    }
    data->position_ = position;
    data->rotation_ = rotation;
    data->scale_ = scale;
    data->rebuildModel();
    for (const auto &graphic : list_graphic_)
    {
        if (graphic->getModeltype() == OperatorAction::CreatAxis)
        {
            graphic->setModelData(data->model_);
        }
    }
    if ((*it)->getModeltype() == OperatorAction::CreatLight)
    {
        syncLight();
    }
}

void ManageEngine::setPropertyData(const Utils::Material &material_)
{

    auto iter = std::find_if(list_graphic_.begin(), list_graphic_.end(),
                             [](GraphicsEnginePtr graphic_) { return (graphic_->getCheck() == true); });
    if (iter != list_graphic_.end())
    {
        (*iter)->setMaterialData(material_);
    }
}

void ManageEngine::setLightSettings(Lighting::LightType type, Lighting::LightMode mode)
{
    if (mode != Lighting::LightMode::Single && mode != Lighting::LightMode::Multiple)
    {
        qWarning() << "Invalid light mode";
        return;
    }
    if (light_config_.lights.empty())
    {
        return;
    }
    auto &p = light_config_.lights.front().parameters;
    switch (type)
    {
    case Lighting::LightType::Directional:
        p = Lighting::DirectionalLight{};
        break;
    case Lighting::LightType::Point:
        p = Lighting::PointLight{};
        break;
    case Lighting::LightType::Spot:
        p = light_spot_;
        break;
    default:
        qWarning() << "Invalid light type";
        return;
    }
    light_config_.mode = mode;
    syncLight();

    // 当前界面只编辑第一盏灯；类型变化时重置其类型专属参数，保留公共光强。
}

void ManageEngine::paintGl()
{
    if (list_graphic_.empty())
    {
        return;
    }

    // All objects receive the same world-space light configuration.
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    for (const auto &graphic_ : list_graphic_)
    {
        if (graphic_->getDefaultShader())
        {
            graphic_->getDefaultShader()->bind();
            // 每个物体都有独立 view 平移，这里把灯光控制器位置换算到当前物体使用的坐标系。
            graphic_->getDefaultShader()->setLights(light_config_);
            graphic_->Draw();
        }
    }
    // 如果模型是空，不进行离屏渲染
    if (list_graphic_.empty())
    {
        return;
    }

    if (screen_render_model_ && screen_render_model_->getRenderType() != 0)
    {
        screen_render_model_->drawTexture();
        for (const auto &graphic_ : list_graphic_)
        {
            MeshPtr mesh = graphic_->getMesh();
            auto *planet = dynamic_cast<PlanetModel *>(graphic_.get());
            screen_render_model_->setScreenRenderVertexData(
                mesh->vao_, mesh->data_.indices, graphic_->getDefaultShader(),
                static_cast<int>(mesh->data_.vertices.size()), planet ? planet->instanceCount() : 0);
            screen_render_model_->Draw();
        }
    }
    if (shadow_map_pass_)
    {
        shadow_map_pass_->beginRenderPass();
        for (const auto& graphic_ : list_graphic_)
        {
            
            MeshPtr mesh = graphic_->getMesh();
            auto* planet = dynamic_cast<PlanetModel*>(graphic_.get());
            shadow_map_pass_->setRenderData(mesh, graphic_->getMvpData(),
                planet ? planet->instanceCount() : 0);
            shadow_map_pass_->Draw();
        }
        shadow_map_pass_->endRenderPass();
    }
}

MvpDataPtr ManageEngine::pickModel(int xpos, int ypos)
{
    MvpDataPtr mvp_data = nullptr;
    if (list_graphic_.empty())
    {
        showAxisModel(false);
        return mvp_data;
    }
    // 清空物体选中状态
    std::for_each(list_graphic_.begin(), list_graphic_.end(), [](auto graphic_) { graphic_->setChecked(false); });

    int object_id = 1;
    std::map<float, GraphicsEnginePtr> map_engineptr;
    for (auto &graphic_ : list_graphic_)
    {
        if (graphic_->getModeltype() == OperatorAction::CreatAxis ||
            graphic_->getModeltype() == OperatorAction::ClearSkyBox)
        {
            continue;
        }
        MvpDataPtr mvp = graphic_->getMvpData();
        MeshPtr mesh = graphic_->getMesh();
        auto *planet = dynamic_cast<PlanetModel *>(graphic_.get());
        screen_render_model_->setScreenRenderVertexData(mesh->vao_, mesh->data_.indices, nullptr,
                                                        static_cast<int>(mesh->data_.vertices.size()),
                                                        planet ? planet->instanceCount() : 0);
        auto [selected, depth] =
            screen_render_model_->colorPick(mvp->model_, mvp->view_, mvp->projection_, xpos, ypos, object_id);
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
        const auto selectedData = map_engineptr.begin()->second->getMvpData();
        for (const auto &graphic : list_graphic_)
        {
            if (graphic->getModeltype() == OperatorAction::CreatAxis)
            {
                graphic->setModelData(selectedData->model_);
            }
        }
        selectModelSignals(map_engineptr.begin()->second->getModelName());
        showAxisModel(true);
        return map_engineptr.begin()->second->getMvpData();
    }
    else
    {
        showAxisModel(false);
        return mvp_data;
    }
}

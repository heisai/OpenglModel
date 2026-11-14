#pragma once
#include "../../Common/OpengHearder.h"
#include <qdebug.h>
#include <QUuid>
#include "../Render/LightConfig.h"
// 命名规范说明：
// - 类名使用 PascalCase（首字母大写）以区分类型与变量。
// - 成员函数使用 camelCase（首字母小写的驼峰式），参数全部小写，便于阅读与一致性。
// - 成员变量以后缀下划线命名（例如 width_），避免与参数名冲突，提高可读性。
// - 常量以 K 开头: KExampleValue。此文件未定义常量，仅说明规则。
// 这样可以统一工程编码风格，减少歧义并方便维护。

class ManageEngine : public QObject
{
    Q_OBJECT
  public:
    // 初始化场景管理器并注册模型与渲染操作。
    ManageEngine();

    // 窗口/视口大小
    // 更新视口尺寸、对象投影矩阵及离屏渲染尺寸。
    void setViewSize(int width, int height);
    // 创建模型
    // 执行创建或渲染操作；创建模型时返回唯一名称。
    QString createModel(OperatorAction type);
    // 删除指定类型或选中模型，或关闭选中模型的爆炸效果。
    QString removeModel(OperatorAction type);
    // 渲染 / 拾取 / 初始化
    // 上传统一世界灯光并绘制场景及已启用的后处理。
    void paintGl();
    // 按视口像素坐标拾取最近模型，更新选择和坐标轴，返回其变换。
    MvpDataPtr pickModel(int xpos, int ypos);
    // 在当前 OpenGL 上下文初始化场景及离屏渲染资源。
    void initializeGl();
    // 设置属性配置信息
    //  更新当前选中模型的材质。
    void setPropertyData(const Utils::Material &material_);
    // Validate and atomically apply the full panel; reject invalid input without changing state.
    bool applyLightPanelData(const Lighting::LightPanelData &data);
    // Update the selected object's independent transform, axes and light state.
    void updateSelectedTransform(const glm::vec3 &position, const glm::mat4 &rotation, const glm::vec3 &scale);
    // Return the primary light's complete editing snapshot for UI feedback.
    Lighting::LightPanelData currentLightPanelData() const
    {
        return light_state_.panel;
    }
    // Read the synchronized world-space configuration used by all renderables.
    const Lighting::LightConfig &lightConfig() const
    {
        return light_state_.config;
    }

  private:
    Lighting::LightState light_state_;
    // 同步缓存参数至第一盏灯、灯体及其坐标轴，并通知界面回显。
    void syncLight();

    // 注册引擎创建函数
    //  注册操作类型与模型工厂、渲染处理函数的对应关系。
    void registerEngineModel();
    // 创建坐标轴
    //  创建仅用于显示的 XYZ 坐标轴模型。
    GraphicsEnginePtr createAxisEngine();
    // 创建网格
    //  创建使用场景灯光的地面网格模型。
    GraphicsEnginePtr createGridEngine();
    // 创建立方体
    //  创建使用场景灯光的立方体模型。
    GraphicsEnginePtr createCubeEngine();
    // 创建光源
    //  创建表示第一盏灯位置的可见灯体模型。
    GraphicsEnginePtr createLightEngine();
    // 创建圆柱体
    //  创建圆环模型并配置场景光照着色器。
    GraphicsEnginePtr createToursEngine();
    // 创建圆环体
    //  创建圆柱模型并配置支持爆炸效果的几何着色器。
    GraphicsEnginePtr createCylinderEngine();
    // 创建天空盒子
    //  创建立方体贴图天空盒模型。
    GraphicsEnginePtr createSkyBoxEngine();
    // 创建行星
    //  创建使用实例矩阵绘制的行星模型。
    GraphicsEnginePtr createPlanteEngine();

    // 反相
    //  启用反色后处理；不创建模型，返回空指针。
    GraphicsEnginePtr createInversionRender();
    // 灰度
    //  启用灰度后处理；不创建模型，返回空指针。
    GraphicsEnginePtr createGrayscaleRender();
    // 瑞化
    //  启用锐化后处理；不创建模型，返回空指针。
    GraphicsEnginePtr createSharpenRender();
    // 模糊
    //  启用模糊后处理；不创建模型，返回空指针。
    GraphicsEnginePtr createBlurRender();
    // 检测
    //  启用边缘检测后处理；不创建模型，返回空指针。
    GraphicsEnginePtr createDetectionRender();
    // 爆破模式
    //  启用选中模型的爆炸效果；不创建模型，返回空指针。
    GraphicsEnginePtr createBlastingRender();
    // 关闭后处理；不创建模型，返回空指针。
    GraphicsEnginePtr clearRender();
	// 创建阴影
    GraphicsEnginePtr createShadowMapPass();
    // GraphicsEnginePtr createInversionRender();

    // 外部模型加载的预留入口，目前未实现。
    void createLoadModelEngine();
    // 混合模型的预留入口，目前未实现。
    void createMixEngine();
    // 展示坐标轴模型
    //  设置坐标轴模型的显示状态。
    void showAxisModel(bool flag);

  protected:
    // 初始化模型变换及共享相机，分配名称并加入场景。
    void addEngine(const GraphicsEnginePtr &graphics);
    // 为模型名称附加递增编号，保证同类模型名称唯一。
    void generateUuid(QString &model_name);
  signals:
    // 选中模型信号
    //  通知界面当前拾取选中的模型名称。
    void selectModelSignals(const QString model_uuid);
    // 通知界面灯光参数已更新，可读取状态进行回显。
    void lightChanged();
    // 通知界面显示未通过校验的原因，失败时不发出 lightChanged。
    void lightEditRejected(const QString &message);

  public:
    std::list<GraphicsEnginePtr> list_graphic_;
    std::map<OperatorAction, std::function<GraphicsEnginePtr(void)>> map_graphicengine_createfunc_;
    int width_ = 0;
    int height_ = 0;
    std::unique_ptr<ScreenRenderModel> screen_render_model_;
    std::unique_ptr<ShadowMapPass> shadow_map_pass_; 
    // 映射模型ID与引擎ID关系
    std::map<QString, unsigned int> map_model_id_;
};

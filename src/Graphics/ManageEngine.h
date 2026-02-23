#pragma once
#include"../Utils/OpengHearder.h"
#include<qdebug.h>
#include <QUuid>
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
    ManageEngine();

    // 窗口/视口大小
    void setViewSize(int width, int height);
    // 创建模型
    QString createModel(OperatorAction type);
    QString removeModel(OperatorAction type);
	// 渲染 / 拾取 / 初始化
	void paintGl();
	MvpDataPtr pickModel(int xpos, int ypos);
	void initializeGl();
	void setEngineScaleAndTranslate(const QString& uuid, const glm::vec3& scale, const glm::vec3& translate, const glm::mat4& model_old);
    void SetEngineScaleAndTranslate(const GraphicsEnginePtr&engine_ptr, const glm::vec3& scale, const glm::vec3& translate, const glm::mat4& model_old);
	//设置属性配置信息
    void setPropertyData(const Utils::Material& material_);
private:

    //注册引擎创建函数
	void registerEngineModel();
	//创建坐标轴
    GraphicsEnginePtr createAxisEngine();
	//创建网格
    GraphicsEnginePtr createGridEngine();
    //创建立方体
    GraphicsEnginePtr createCubeEngine();
    //创建圆柱体
    GraphicsEnginePtr createToursEngine();
    //创建圆环体
    GraphicsEnginePtr createCylinderEngine();
    //创建天空盒子
    GraphicsEnginePtr createSkyBoxEngine();
	//创建行星
    GraphicsEnginePtr createPlanteEngine();

    //反相
    GraphicsEnginePtr createInversionRender();
    //灰度
    GraphicsEnginePtr createGrayscaleRender();
    //瑞化
    GraphicsEnginePtr createSharpenRender();
    //模糊
    GraphicsEnginePtr createBlurRender();
    //检测
    GraphicsEnginePtr createDetectionRender();
    //爆破模式
	GraphicsEnginePtr createBlastingRender();
    GraphicsEnginePtr clearRender();
    //GraphicsEnginePtr createInversionRender();

    void createLoadModelEngine();
    void createMixEngine();
	//展示坐标轴模型
    void showAxisModel(bool flag);
protected:
    void addEngine( const GraphicsEnginePtr& graphics);
    void  generateUuid(QString &model_name);
signals:
    //选中模型信号
    void selectModelSignals(const QString model_uuid);
public:
    std::list<GraphicsEnginePtr>list_graphic_;
    std::map<OperatorAction, std::function<GraphicsEnginePtr(void)>> map_graphicengine_createfunc_;
    int width_ = 0;
    int height_ = 0;
    std::unique_ptr<ScreenRenderModel>screen_render_model_;
	//映射模型ID与引擎ID关系
	std::map<QString, unsigned int>map_model_id_;
};


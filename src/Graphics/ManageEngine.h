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
public:
    ManageEngine();

    // 窗口/视口大小
    void setViewSize(int width, int height);

    // 创建模型
    void createModel(EngineType type);
    void createGridEngine();
    GraphicsEnginePtr createCubeEngine();
    GraphicsEnginePtr createToursEngine();
    GraphicsEnginePtr createCylinderEngine();
    void createLoadModelEngine();
    void createMixEngine();

    // 移除引擎
    void removeEngine(EngineType type);

    // 复选框响应： type 为 uuid 字符串
    void checkBoxTypeSlot(const QString& type, bool check);

    // 渲染 / 拾取 / 初始化
    void paintGl();
    MvpDataPtr pickModel(int xpos, int ypos);
    void initializeGl();
    void setEngineScaleAndTranslate(const QString& uuid, const glm::vec3& scale, const glm::vec3& translate, const glm::mat4& model_old);

protected:
    void addEngine( const GraphicsEnginePtr& graphics);
    QString generateUuid();

public:
    std::map<QString, GraphicsEnginePtr> map_graphic_;
    int width_ = 0;
    int height_ = 0;
};


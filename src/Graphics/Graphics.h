#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H
#include<iostream>
#include<filesystem>
#include<vector>
#include<memory>
#include<tuple>
#include<QVector2D>
// // #define STB_IMAGE_IMPLEMENTATION

#include"Shader.h"
#include"../ModelDatas/ModelDataInterFace.h"
#include<QOpenGLFunctions_4_5_Core>
/*
    1: 图形引擎基类
    2: Draw: 采用多态机制 运行时绑定
*/
struct MvpData
{
    glm::mat4 model_ = glm::mat4(1.0f);
    glm::mat4 view_ = glm::mat4(1.0f);
    glm::mat4 projection_ = glm::mat4(1.0f);

    //平移坐标
	QVector2D tranlstor_position_;
    //平移坐标备份
    QVector2D tranlstor_position_temp_;
};
using MvpDataPtr = std::shared_ptr<MvpData>;


class GraphicsEngine:public QOpenGLFunctions_4_5_Core
{
public:
    GraphicsEngine(std::shared_ptr<Shader> shader = nullptr, std::shared_ptr<Shader>m_Stencilshader = nullptr);
    virtual void Draw() = 0;
    virtual void InitBufferData() = 0;
    virtual void setModelData(const ModelDataInfo& datas) = 0;
    virtual void setScreenRenderVertexData(unsigned int vao, std::vector<unsigned int>indices) {};
    virtual bool colorPick(glm::mat4 model, glm::mat4 view, glm::mat4 projection, int xpos, int ypos,int objetc_id)
    {
        return false;
    }
    virtual void SetViewSize(int width, int height);
    void UseShader();
    void setChecked(bool flag);

    /*virtual ~ GraphicsEngine();*/
public:
     std::shared_ptr<Shader>m_shader;
     std::shared_ptr<Shader>m_PickShader;
     std::shared_ptr<Shader>stencil_shader_;
     


     int m_Width, m_Height;


     MvpDataPtr mvp_data_;



};
using GraphicsEnginePtr = std::shared_ptr<GraphicsEngine>;

#endif //GRAPHICSENGINE_H

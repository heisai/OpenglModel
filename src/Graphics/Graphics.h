#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H
#include<iostream>
#include<filesystem>
#include<vector>
#include<memory>
#include<tuple>
#include<QVector2D>


#include"Shader.h"
#include"../ModelDatas/CreatModelData.h"
#include <QtOpenGL/QOpenGLFunctions_4_5_Core>

/*
    1: 图形引擎基类
    2: Draw: 采用多态机制 运行时绑定
*/
//世界空间坐标
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
//网格基础单元
struct Mesh
{
    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;
	std::vector<float> vertices_datas; //顶点数据
	std::vector<unsigned int>indices_datas;
    EngineType model_type_;   //模型类型
};
using MeshPtr = std::shared_ptr<Mesh>;


class GraphicsEngine:public QOpenGLFunctions_4_5_Core
{
public:
    GraphicsEngine(EngineType type,std::shared_ptr<Shader> shader = nullptr, std::shared_ptr<Shader>m_Stencilshader = nullptr);
    virtual void Draw() = 0;
    virtual void InitBufferData() = 0;
    
    virtual void setScreenRenderVertexData(unsigned int vao, std::vector<unsigned int>indices) {};
    virtual bool colorPick(glm::mat4 model, glm::mat4 view, glm::mat4 projection, int xpos, int ypos,int objetc_id)
    {
        return false;
    }
    virtual void SetViewSize(int width, int height);
    void UseShader();
    void setChecked(bool flag);


    void setViewData(const glm::mat4& view_);
    void setModelData(const glm::mat4& model_);
    void setProjectionData(const glm::mat4& projection_);
    void setTranlstorPosition(const QVector2D& tranlstor_position_);
    MvpDataPtr getMvpData();
    MeshPtr getMesh();
    EngineType getModeltype()const;
    bool getCheck()const;
    /*virtual ~ GraphicsEngine();*/
private:
    void setModelInfo(const ModelDataInfo& model_datas);
public:
     std::shared_ptr<Shader>m_shader;
     std::shared_ptr<Shader>m_PickShader;
     std::shared_ptr<Shader>stencil_shader_;
     



    
protected:
  
	 int m_Width, m_Height;
	 MvpDataPtr mvp_data_;      //世界空间坐标
     MeshPtr mesh_data_;           //模型数据
     bool selected_ = false;        //选中状态
     EngineType model_type_;   //模型类型

};
using GraphicsEnginePtr = std::shared_ptr<GraphicsEngine>;

#endif //GRAPHICSENGINE_H

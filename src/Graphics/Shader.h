#ifndef SHADER_H
#define SHADER_H


#include"../Utils/Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QtOpenGL/QOpenGLFunctions_4_5_Core>
#include "../Utils/Utils.h"
#include"../ModelDatas/ModelDataInterFace.h"
class Shader: public QOpenGLFunctions_4_5_Core {
public:
    explicit Shader(const std::string& vs_filename,const std::string &fs_filename,
		std::string dirname);
	explicit Shader(const std::string& vs_filename, const std::string& fs_filename,
        const std::string& gs_filename,const std::string& dirname);
   

    //绑定着色器
    void bind();
    //解绑着色器
    void release();
	//设置材质属性
    void setMaterial(const Utils::Material& material_info);
	//设置光照属性
	//void setLight(const Utils::Light& light_info);
    void setBool(const std::string& name, bool value) ;
    void SetInt(const std::string& name, int value) ;
    void SetuInt(const std::string& name, unsigned int value) ;
    void setFloat(const std::string& name, float value) ;
    void setVec2(const std::string& name, const glm::vec2& value) ;
    void setVec2(const std::string& name, float x, float y) ;
    void setVec3(const std::string& name, const glm::vec3& value) ;
    void setVec3(const std::string& name, float x, float y, float z) ;
    void setVec4(const std::string& name, const glm::vec4& value) ;
    void setVec4(const std::string& name, float x, float y, float z, float w) ;
    void setMat2(const std::string& name, const glm::mat2& mat) ;
    void setMat3(const std::string& name, const glm::mat3& mat) ;
    void setMat4(const std::string& name, const glm::mat4& mat) ;


	void CreatProgram();
     char* vShaderCode = nullptr ;
     char* fShaderCode = nullptr;
     char* gShaderCode = nullptr;
protected:
   
    void CreatShader();
    void BindShader();

public:
    unsigned int ShaderPromger = 0;
    unsigned int VertecShader = 0;
    unsigned int FragmentShader = 0;
    unsigned int geometry_shader_ = 0;

	std::string vs_sourcecode;  //顶点着色器源码
	std::string fs_sourcecode;  //片段着色器源码
	std::string gs_sourcecode;//几何着色器源码

    GLuint phong_index_;
    GLuint blinn_index_;

};
using ShaderPtr = std::shared_ptr<Shader>;
#endif // SHADER_H

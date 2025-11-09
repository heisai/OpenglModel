#ifndef SHADER_H
#define SHADER_H


#include"../Utils/Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<QOpenGLFunctions_4_5_Core>
class Shader: public QOpenGLFunctions_4_5_Core {
public:
    explicit Shader(const std::string& vs_filename,const std::string &fs_filename,
		std::string dirname = "");
   
	std::string GetDirname()
	{
		return m_DirName;
	}

    void bind();
    void release();
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
    const char* vShaderCode;
    const char* gShaderCode;
protected:
   
    void CreatShader();
    void BindShader();

public:
    unsigned int ShaderPromger = 0;
    unsigned int VertecShader = 0;
    unsigned int FragmentShader = 0;
	std::string m_DirName;
    std::string vs_sourcecode;
    std::string fs_sourcecode;

};
using ShaderPtr = std::shared_ptr<Shader>;
#endif // SHADER_H

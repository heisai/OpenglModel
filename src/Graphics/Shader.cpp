#include"Shader.h"

Shader::Shader(const std::string &vs_filename, const std::string &fs_filename, std::string dirname):
    m_DirName(dirname)
{
   
    initializeOpenGLFunctions();
     vs_sourcecode = Utils::ReadFile(Utils::GlslAbsolute(vs_filename, dirname));
     fs_sourcecode = Utils::ReadFile(Utils::GlslAbsolute(fs_filename, dirname));
    vShaderCode = vs_sourcecode.c_str();
    gShaderCode = fs_sourcecode.c_str();
    std::cout<<vs_sourcecode<<std::endl;
    std::cout<<fs_sourcecode<<std::endl;
    
   
}

void Shader::UseProgram()
{
    glUseProgram(ShaderPromger);
}

void Shader::bind()
{
    glUseProgram(ShaderPromger);
}

void Shader::release()
{

}

void Shader::setBool(const std::string &name, bool value) 
{
    glUniform1i(glGetUniformLocation(ShaderPromger, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) 
{
    glUniform1i(glGetUniformLocation(ShaderPromger, name.c_str()), value);
}

void Shader::SetuInt(const std::string& name, unsigned int value) 
{
    glUniform1i(glGetUniformLocation(ShaderPromger, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) 
{
    glUniform1f(glGetUniformLocation(ShaderPromger, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) 
{
    glUniform2fv(glGetUniformLocation(ShaderPromger, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) 
{
    glUniform2f(glGetUniformLocation(ShaderPromger, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) 
{
    glUniform3fv(glGetUniformLocation(ShaderPromger, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) 
{
    glUniform3f(glGetUniformLocation(ShaderPromger, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) 
{
    glUniform4fv(glGetUniformLocation(ShaderPromger, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) 
{
    glUniform4f(glGetUniformLocation(ShaderPromger, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) 
{
    glUniformMatrix2fv(glGetUniformLocation(ShaderPromger, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) 
{
    glUniformMatrix3fv(glGetUniformLocation(ShaderPromger, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(ShaderPromger, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::CreatProgram()
{

  
    this->CreatShader();
    this->BindShader();
}

void Shader::CreatShader()
{
    VertecShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertecShader,1,&vShaderCode,NULL);
     glCompileShader(VertecShader);
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader,1,&gShaderCode,NULL);

    glCompileShader(FragmentShader);

}
void Shader::BindShader()
{
    ShaderPromger = glCreateProgram();
    glAttachShader(ShaderPromger,VertecShader);
    glAttachShader(ShaderPromger,FragmentShader);
    glLinkProgram(ShaderPromger);


    // glDeleteShader(VertecShader);
    // glDeleteShader(FragmentShader);



}



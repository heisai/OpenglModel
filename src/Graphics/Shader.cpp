#include"Shader.h"

Shader::Shader(const std::string &vs_filename, const std::string &fs_filename, std::string dirname)
{
   
    initializeOpenGLFunctions();
     vs_sourcecode = Utils::ReadFile(Utils::joinPaths(dirname, vs_filename));
     fs_sourcecode = Utils::ReadFile(Utils::joinPaths( dirname, fs_filename));
    vShaderCode = vs_sourcecode.data();
    fShaderCode = fs_sourcecode.data();
   
}
Shader::Shader(const std::string& vs_filename, const std::string& fs_filename, const std::string& gs_filename, const std::string& dirname)
{
	initializeOpenGLFunctions();
	vs_sourcecode = Utils::ReadFile(Utils::joinPaths(dirname, vs_filename));
	fs_sourcecode = Utils::ReadFile(Utils::joinPaths(dirname, fs_filename));
    gs_sourcecode = Utils::ReadFile(Utils::joinPaths(dirname, gs_filename));
	vShaderCode = vs_sourcecode.data();
	fShaderCode = fs_sourcecode.data();
    gShaderCode = gs_sourcecode.data();
    //LogInfo("===========Vert GLSL=========");
}

void Shader::bind()
{
    glUseProgram(ShaderPromger);
}

void Shader::release()
{
    glUseProgram(0);
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
    glShaderSource(FragmentShader,1,&fShaderCode,NULL);
    glCompileShader(FragmentShader);

    if (gShaderCode)
    {
		geometry_shader_ = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_shader_, 1, &gShaderCode, NULL);
		glCompileShader(geometry_shader_);
    }


}
void Shader::BindShader()
{
    ShaderPromger = glCreateProgram();
    glAttachShader(ShaderPromger,VertecShader);
    glAttachShader(ShaderPromger,FragmentShader);
    if (gShaderCode)
    {
        glAttachShader(ShaderPromger, geometry_shader_);
    }
  
    glLinkProgram(ShaderPromger);


    // glDeleteShader(VertecShader);
    // glDeleteShader(FragmentShader);



}



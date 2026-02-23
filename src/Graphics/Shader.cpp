#include"Shader.h"

Shader::Shader(const std::string &vs_filename, const std::string &fs_filename, std::string dirname)
{
   
    initializeOpenGLFunctions();
    std::string vs_path = Utils::joinPaths(dirname, vs_filename);
    std::string fs_path = Utils::joinPaths(dirname, fs_filename);
    // 调试：打印实际加载的着色器路径
    std::cout << "[Shader] Loading VS: " << vs_path << std::endl;
    std::cout << "[Shader] Loading FS: " << fs_path << std::endl;
    
    vs_sourcecode = Utils::ReadFile(vs_path);
    fs_sourcecode = Utils::ReadFile(fs_path);
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

void Shader::setMaterial(const Utils::Material& material_info)
{
	setVec3("Material.Ka", material_info.ambient_);
	setVec3("Material.Kd", material_info.diffsue_);
	setVec3("Material.Ks", material_info.specular_);
	setFloat("Material.Shininess", material_info.shininess_);
    setBool("flatShading", material_info.render_type_);
    if (shade_model_location_ >= 0 && num_subroutine_uniforms_ > 0)
    {
        GLuint active_index = material_info.light_model_type_ ? phong_index_ : blinn_index_;
        if (active_index != GL_INVALID_INDEX)
        {
            subroutine_indices_[shade_model_location_] = active_index;
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, num_subroutine_uniforms_, subroutine_indices_.data());
        }
    }
    //LogInfo("Set Light Model Type: %d", material_info.light_model_type_);
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

    phong_index_ = glGetSubroutineIndex(ShaderPromger, GL_FRAGMENT_SHADER, "phongModel");
    blinn_index_ = glGetSubroutineIndex(ShaderPromger, GL_FRAGMENT_SHADER, "blinnModel");
    if (phong_index_ == GL_INVALID_INDEX || blinn_index_ == GL_INVALID_INDEX)
    {
        std::cerr << "[Shader] Warning: subroutine indices not found (phong="
                  << phong_index_ << " blinn=" << blinn_index_ << ")" << std::endl;
    }
    shade_model_location_ = glGetSubroutineUniformLocation(ShaderPromger, GL_FRAGMENT_SHADER, "shadeModel");
    glGetProgramStageiv(ShaderPromger, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &num_subroutine_uniforms_);
    subroutine_indices_.assign(num_subroutine_uniforms_, 0);
    // glDeleteShader(VertecShader);
    // glDeleteShader(FragmentShader);



}



#include"Shader.h"
namespace fs = std::filesystem;
Shader::Shader(const std::string &vs_filename, const std::string &fs_filename, std::string dirname)
{
    initializeOpenGLFunctions();
    std::string graphics_root = fs::current_path().parent_path().string() + "\\src\\Graphics\\";
    std::string vs_path = Utils::joinPaths(dirname, vs_filename);
    std::string fs_path = Utils::joinPaths(dirname, fs_filename);
    std::cout << "[Shader] Loading VS: " << vs_path << std::endl;
    std::cout << "[Shader] Loading FS: " << fs_path << std::endl;

    vs_sourcecode = Utils::resolveIncludes(Utils::ReadFile(vs_path), graphics_root);
    fs_sourcecode = Utils::resolveIncludes(Utils::ReadFile(fs_path), graphics_root);
    vShaderCode = vs_sourcecode.data();
    fShaderCode = fs_sourcecode.data();

	LogInfo("vShaderCode: {}", vShaderCode);
	LogInfo("fShaderCode: {}", fShaderCode);
}
Shader::Shader(const std::string& vs_filename, const std::string& fs_filename, const std::string& gs_filename, const std::string& dirname)
{
	initializeOpenGLFunctions();
	std::string graphics_root = fs::current_path().parent_path().string() + "\\src\\Graphics\\";
	vs_sourcecode = Utils::resolveIncludes(Utils::ReadFile(Utils::joinPaths(dirname, vs_filename)), graphics_root);
	fs_sourcecode = Utils::resolveIncludes(Utils::ReadFile(Utils::joinPaths(dirname, fs_filename)), graphics_root);
	gs_sourcecode = Utils::resolveIncludes(Utils::ReadFile(Utils::joinPaths(dirname, gs_filename)), graphics_root);
	vShaderCode = vs_sourcecode.data();
	fShaderCode = fs_sourcecode.data();
	gShaderCode = gs_sourcecode.data();
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
    SetInt("flatShading", material_info.render_type_);
    if (material_info.light_model_type_)
    {
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &phong_index_);
    }
    else
    {
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &blinn_index_);
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
	glShaderSource(VertecShader, 1, &vShaderCode, NULL);
	glCompileShader(VertecShader);
	GLint ok = 0;
	glGetShaderiv(VertecShader, GL_COMPILE_STATUS, &ok);
	if (!ok)
	{
		GLint n = 0;
		glGetShaderiv(VertecShader, GL_INFO_LOG_LENGTH, &n);
		std::string log(n, '\0');
		glGetShaderInfoLog(VertecShader, n, nullptr, log.data());
		std::cout << "[GLSL] VertexShader error:\n" << log << std::endl;
	}

	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(FragmentShader);
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &ok);
	if (!ok)
	{
		GLint n = 0;
		glGetShaderiv(FragmentShader, GL_INFO_LOG_LENGTH, &n);
		std::string log(n, '\0');
		glGetShaderInfoLog(FragmentShader, n, nullptr, log.data());
		std::cout << "[GLSL] FragmentShader error:\n" << log << std::endl;
	}

	if (gShaderCode)
	{
		geometry_shader_ = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_shader_, 1, &gShaderCode, NULL);
		glCompileShader(geometry_shader_);
		glGetShaderiv(geometry_shader_, GL_COMPILE_STATUS, &ok);
		if (!ok)
		{
			GLint n = 0;
			glGetShaderiv(geometry_shader_, GL_INFO_LOG_LENGTH, &n);
			std::string log(n, '\0');
			glGetShaderInfoLog(geometry_shader_, n, nullptr, log.data());
			std::cout << "[GLSL] GeometryShader error:\n" << log << std::endl;
		}
	}
}

void Shader::BindShader()
{
	ShaderPromger = glCreateProgram();
	glAttachShader(ShaderPromger, VertecShader);
	glAttachShader(ShaderPromger, FragmentShader);
	if (gShaderCode)
	{
		glAttachShader(ShaderPromger, geometry_shader_);
	}

	glLinkProgram(ShaderPromger);
	GLint ok = 0;
	glGetProgramiv(ShaderPromger, GL_LINK_STATUS, &ok);
	if (!ok)
	{
		GLint n = 0;
		glGetProgramiv(ShaderPromger, GL_INFO_LOG_LENGTH, &n);
		std::string log(n, '\0');
		glGetProgramInfoLog(ShaderPromger, n, nullptr, log.data());
		std::cout << "[GLSL] Program link error:\n" << log << std::endl;
	}

	phong_index_ = glGetSubroutineIndex(ShaderPromger, GL_FRAGMENT_SHADER, "phongModel");
	blinn_index_ = glGetSubroutineIndex(ShaderPromger, GL_FRAGMENT_SHADER, "blinnModel");
	auto ltrim = [](std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c){ return !std::isspace(c); }));};
	ltrim(fs_sourcecode);
}



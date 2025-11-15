#ifndef UTILS_H
#define UTILS_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<filesystem>
#include<memory>
#include<glm/glm.hpp>
#include<map>
#include<string>
#include<format>
namespace fs = std::filesystem;



namespace  Utils {

    //读取文件内容
    std::string ReadFile(const std::string &file_name);
    // 生成着色其内容
    void CreatShaderProgram(const std::string &vs, const std::string &fs);
	template<typename... Args>
	std::string joinPaths(Args&&... args) 
    {
		fs::path result;
		(result /= ... /= fs::path(std::forward<Args>(args)));
		std::string current_dir = fs::current_path().string();
		const size_t  last_slash_idx = current_dir.rfind('\\');
		if (std::string::npos != last_slash_idx)
		{
			current_dir = current_dir.substr(0, last_slash_idx);
		}
        std::string file_path =  std::format("{}\\src\\Graphics\\{}", current_dir, result.string());
	    return file_path;
	}
    class MaterialAttrib
    {
    public:
        MaterialAttrib(const std::string& attrib);
        void SetMaterialAttrib(const std::string& attrib);
        MaterialAttrib() = delete;
        glm::vec3 GetAmbient() ;

        glm::vec3 GetDiffsue();

        glm::vec3 GetSpecular();

        glm::vec4 GetAmbientExt();

        glm::vec4 GetDiffsueExt();

        glm::vec4 GetSpecularExt();

        float GetShininess();
    private:
        class Material
        {
        public:
            glm::vec4 m_Ambient;
            glm::vec4 m_Diffsue;
            glm::vec4 m_Specular;
 
            float   m_Shininess;
        };

        std::map<std::string, Material>m_MapMaterial;
        std::string m_MaterialName;

    };

}

#endif // UTILS_H

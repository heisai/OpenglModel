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
#include"Translator.h"
namespace fs = std::filesystem;

namespace Utils {

    // 读取文件内容到字符串（用于读取 GLSL 文件等）
    std::string ReadFile(const std::string& file_name);

    // 读取顶点/片元着色器内容（当前仅做读取动作，便于后续扩展）
    void CreatShaderProgram(const std::string& vs, const std::string& fs);

    // 拼接资源路径：将 dirname + shader filename 组合为工程内的实际磁盘路径
    // 说明：当前实现依赖运行时工作目录（current_path）
    template<typename... Args>
    std::string joinPaths(Args&&... args)
    {
        fs::path result;
        (result /= ... /= fs::path(std::forward<Args>(args)));

        std::string current_dir = fs::current_path().string();
        const size_t last_slash_idx = current_dir.rfind('\\');
        if (std::string::npos != last_slash_idx)
        {
            current_dir = current_dir.substr(0, last_slash_idx);
        }
        std::string file_path = std::format("{}\\src\\Graphics\\{}", current_dir, result.string());
        return file_path;
    }

	// 单个材质条目（Phong/Blinn-Phong 常用四元组）
		// ambient_  : 环境光反射系数
		// diffsue_  : 漫反射系数（拼写沿用原代码 diffsue）
		// specular_ : 镜面反射系数
		// shininess_: 高光指数（越大高光越集中）
	class Material
	{
	public:
		glm::vec3 ambient_;
		glm::vec3 diffsue_;
		glm::vec3 specular_;
		float shininess_{};
        float alpha_{};
	};

    // 材质属性表：根据材质名称（key）返回对应的环境光/漫反射/高光/光泽度
    // 用途：为着色器提供统一的材质参数（Phong/Blinn-Phong 等）
    class MaterialAttrib
    {
    public:
        // attrib: 材质名称 key，例如 "gold" / "emerald" / "black_plastic"
        explicit MaterialAttrib();
       ~MaterialAttrib() = default;

        Material getMaterial(const QString& name);
    private:
        // 材质名称 -> 材质参数表
        std::map<QString, Material> map_material_;
    };

}

#endif // UTILS_H

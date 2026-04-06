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
#include<unordered_set>
#include"Translator.h"
namespace fs = std::filesystem;

namespace Utils {

    // 读取文件内容到字符串（用于读取 GLSL 文件等）
    std::string ReadFile(const std::string& file_name);

    // GLSL #include 预处理器
    // 将 source 中所有 #include "path" 指令展开为对应文件的内容
    // path 相对于 src/Graphics/ 根目录
    // 内部使用 included_files 防止重复包含（类似 #pragma once）
    std::string resolveIncludes(const std::string& source,
                                const std::string& graphics_root,
                                std::unordered_set<std::string>& included_files,
                                int depth = 0);
    std::string resolveIncludes(const std::string& source,
                                const std::string& graphics_root);

    // 读取顶点/片元着色器内容（当前仅做读取动作，便于后续扩展）
    void CreatShaderProgram(const std::string& vs, const std::string& fs);

    // 拼接资源路径：将 dirname + shader filename 组合为工程内的实际磁盘路径
    // 说明：当前实现依赖运行时工作目录（current_path）
    template<typename... Args>
    std::string joinPaths(Args&&... args)
    {
        fs::path result;
        (result /= ... /= fs::path(std::forward<Args>(args)));

        // 使用项目根目录的绝对路径，确保着色器文件从源码目录加载
        std::string project_root = fs::current_path().parent_path().string();
        std::string file_path = std::format("{}\\src\\Graphics\\{}", project_root, result.string());
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
        Material() = default;

        Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, float alpha = 1.0f, int lightModelType = 0)
            : ambient_(ambient), diffsue_(diffuse), specular_(specular), shininess_(shininess), alpha_(alpha), light_model_type_(lightModelType)
        {
        }

        //材质属性
		glm::vec3 ambient_{ 0.0f };
		glm::vec3 diffsue_{ 0.0f };
		glm::vec3 specular_{ 0.0f };
		float shininess_{ 0.0f };
        float alpha_{ 1.0f };
        //光照类型
        int light_model_type_{ 0 };  // 0 :phong    1：blinn
		int render_type_{ 0 }; // 0: 双面遮光   1： 单面遮光     2: 移除碎片
        // Grid pattern parameters for model selection effect (render_type_ == 2)
        float selection_scale_{ 15.0f };     // Grid frequency: higher = finer grid
        float selection_threshold_{ 0.2f };  // Grid line thickness as fraction of cell size
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

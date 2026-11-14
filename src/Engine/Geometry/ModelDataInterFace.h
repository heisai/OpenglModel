#ifndef MODELDATAINTERFACE_H
#define MODELDATAINTERFACE_H
#include <iostream>
#include <vector>
#include <memory>
#include <cstddef>
#include <cstdint>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <glm/glm.hpp>
#include <QString>
#include "../../Common/Translator.h"
inline void InitLogging()
{
	try {
		auto logger = spdlog::stdout_color_mt("OpenGlEngine");
		//logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [%s:%# %!] %v");
		logger->set_pattern("[%^%l%$] [%s:%# %!] %v");
		spdlog::set_default_logger(logger);
		spdlog::set_level(spdlog::level::info);
		spdlog::flush_on(spdlog::level::info);
		//// 设置控制台输出编码为 UTF-8
		//SetConsoleOutputCP(CP_UTF8);
		//// 可选：同时设置输入编码，保证一致性
		//SetConsoleCP(CP_UTF8);
	}
	catch (...) {
		// 如果多次初始化导致异常，这里捕获以防程序崩溃
	}
}
template <>
struct fmt::formatter<glm::vec3> {
	constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::vec3& v, FormatContext& ctx) {
		// 输出格式为 (x, y, z)
		return fmt::format_to(ctx.out(), "({:.3f}, {:.3f}, {:.3f})", v.x, v.y, v.z);
	}
};
// 宏封装（替代 SPDLOG_INFO / SPDLOG_WARN / SPDLOG_ERROR）
#define LogInfo(...)  SPDLOG_INFO(__VA_ARGS__)
#define LogWarn(...)  SPDLOG_WARN(__VA_ARGS__)
#define LogError(...) SPDLOG_ERROR(__VA_ARGS__)
// 模型操作类型（创建、渲染、清理）
enum class OperatorAction
{
	// 离屏渲染
	ScreenRender = 0,
	// 模型创建动作
	CreatCyliner ,// 创建圆柱体
	CreatCube,// 创建立方体
	CreatTourse,// 创建圆环体
	CreatGrid,	//网格
	CreatAxis,//	坐标
	CreatPlanet,//	行星
	CreatLight,//灯光
	// 渲染动作
	RenderInversion,   // 颜色反相
	RenderGrayscale,   // 灰度
	RenderSharpen,   // 锐化
	RenderBlur,   // 模糊
	RenderDetection,   // 边缘检测
	RenderSky,	//天空盒
	RenderBlasting,	//爆破
	RenderShadow, // 阴影
	// 清除动作
	ClearSkyBox,//清除天空盒
	ClearCreatModel,//清除模型
	ClearBlasting,	//清除爆破
	ClearRender	//清除渲染
};
enum class PrimitiveTopology {
	Triangles, // 三角形图元
	Lines, // 线段图元
	Points // 点图元
};
struct Vertex
{
	glm::vec3 position{};	//位置
	glm::vec3 normal{};		//法向量
	glm::vec2 texCoord{};	//纹理
	glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };	//颜色
};

struct MeshData
{
	PrimitiveTopology topology = PrimitiveTopology::Triangles;//图元类型
	std::vector<Vertex> vertices;	//顶点坐标
	std::vector<uint32_t> indices;	//索引
	std::string model_name_; //模型名称
};


// 模型数据接口：统一获取网格数据
class ModelDataInterFace{
public:
    virtual ~ModelDataInterFace() = default;
	// 获取模型的网格数据
    virtual MeshData GetModelDatas() = 0;
};

#endif // MODELDATAINTERFACE_H

#ifndef MODELDATAINTERFACE_H
#define MODELDATAINTERFACE_H
#include <iostream>
#include <vector>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <glm/glm.hpp>
#include <QString>
#include "../Utils/Translator.h"
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
//模型类型
enum class OperatorAction
{
	//离屏渲染
	ScreenRender = 0,
	//模型动作
	CreatCyliner ,//	
	CreatCube,
	CreatTourse,
	CreatGrid,
	CreatAxis,//	
	CreatPlanet,//	行星
	//渲染动作
	RenderInversion,   //
	RenderGrayscale,   //
	RenderSharpen,   //
	RenderBlur,   //
	RenderDetection,   //
	RenderSky,	//天空盒
	RenderBlasting,	//爆破
	//清除动作
	ClearSkyBox,//清除天空盒
	ClearCreatModel,//清除模型
	ClearBlasting,	//清除爆破
	ClearRender	//清除渲染
};
struct ModelDataInfo
{
	std::vector<float> vertices_datas; //顶点数据
	std::vector<unsigned int>indices_datas;
	QString model_name_; //模型名称
};

class ModelDataInterFace{
public:
    virtual ~ModelDataInterFace() = default;
    virtual ModelDataInfo GetModelDatas() = 0;
};

#endif // MODELDATAINTERFACE_H

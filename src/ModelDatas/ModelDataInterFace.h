#ifndef MODELDATAINTERFACE_H
#define MODELDATAINTERFACE_H
#include<iostream>
#include<vector>
#include<memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <glm/glm.hpp>


inline void InitLogging()
{
	try {
		auto logger = spdlog::stdout_color_mt("OpenGlEngine");
		//logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [%s:%# %!] %v");
		logger->set_pattern("[%^%l%$] [%s:%# %!] %v");
		spdlog::set_default_logger(logger);
		spdlog::set_level(spdlog::level::info);
		spdlog::flush_on(spdlog::level::info);
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

enum EngineType
{
    EM_GRIDENGINE = 0,	//网格引擎
    EM_CUBEENGINE,	//立方体引擎
    EM_TOURSENGINE, //圆环体引擎
    EM_CYLINDERENGINE, //圆柱体引擎
    EM_MODELENGINE,//模型引擎
    EM_MIXENGINE,   //混合引擎
};

struct ModelDataInfo
{

    std::vector<float> m_Vertices;
    std::vector<unsigned int>m_Indices;
};

class ModelDataInterFace{
public:
    virtual ~ModelDataInterFace() = default;
    virtual ModelDataInfo GetModelDatas() = 0;
};

#endif // MODELDATAINTERFACE_H

#include"Utils.h"

std::string Utils::ReadFile(const std::string& file_name)
{
    std::ifstream file;
    std::stringstream file_stream;

    // 读取失败时抛异常（便于在 catch 中统一处理）
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        file.open(file_name);
        file_stream << file.rdbuf();
        file.close();
    }
    catch (std::ifstream::failure& ex)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << ex.what() << std::endl;
    }

    return file_stream.str();
}

void Utils::CreatShaderProgram(const std::string& vs, const std::string& fs)
{
    // 当前函数用于预读取 shader 源码，后续可扩展为实际编译/缓存等流程
    std::string vs_text = ReadFile(vs);
    std::string fs_text = ReadFile(fs);
}

std::string Utils::resolveIncludes(const std::string& source,
                                    const std::string& graphics_root,
                                    std::unordered_set<std::string>& included_files,
                                    int depth)
{
    constexpr int kMaxDepth = 16;
    if (depth > kMaxDepth)
    {
        std::cout << "[Shader] ERROR: #include depth exceeded " << kMaxDepth << std::endl;
        return source;
    }

    std::istringstream stream(source);
    std::ostringstream output;
    std::string        line;

    while (std::getline(stream, line))
    {
        auto pos = line.find("#include");
        if (pos != std::string::npos)
        {
            auto q1 = line.find('"', pos + 8);
            auto q2 = (q1 != std::string::npos) ? line.find('"', q1 + 1) : std::string::npos;

            if (q1 != std::string::npos && q2 != std::string::npos)
            {
                std::string include_path = line.substr(q1 + 1, q2 - q1 - 1);
                // 将 / 换成系统路径分隔符
                std::string full_path = graphics_root + include_path;
                std::replace(full_path.begin(), full_path.end(), '/', '\\');

                if (included_files.count(full_path))
                {
                    // 已包含过，跳过（防重复）
                    continue;
                }
                included_files.insert(full_path);

                std::string included_source = ReadFile(full_path);
                if (included_source.empty())
                {
                    std::cout << "[Shader] ERROR: cannot find include: " << full_path << std::endl;
                    output << line << "\n";
                    continue;
                }
                std::cout << "[Shader] #include resolved: " << include_path << std::endl;
                output << resolveIncludes(included_source, graphics_root, included_files, depth + 1);
                continue;
            }
        }
        output << line << "\n";
    }
    std::cout << output.str() << std::endl;
    return output.str();
}

std::string Utils::resolveIncludes(const std::string& source,
                                    const std::string& graphics_root)
{
    std::unordered_set<std::string> included_files;
    return resolveIncludes(source, graphics_root, included_files, 0);
}

Utils::MaterialAttrib::MaterialAttrib()
{

    // 初始化材质表（map_material_）
    // 说明：
    // 1) 所有材质使用一套统一字段：ambient_/diffsue_/specular_/shininess_
    // 2) 参数范围一般在 [0,1]；shininess_ 为高光指数
    // 3) 这些数据常用于 Phong/Blinn-Phong 光照模型

    // ==============================
    // 经典材质（宝石/金属）
    // ==============================

    // 翠（Emerald）
    Material emerald;
    emerald.ambient_ = glm::vec3(0.0215f, 0.1745f, 0.0215f);
    emerald.diffsue_ = glm::vec3(0.07568f, 0.61424f, 0.07568f);
    emerald.specular_ = glm::vec3(0.633f, 0.727811f, 0.633f);
    emerald.shininess_ = 76.8f;
	emerald.alpha_ = 0.55f;

    // 玉（Jade）
    Material jade;
    jade.ambient_ = glm::vec3(0.135f, 0.2225f, 0.1575f);
    jade.diffsue_ = glm::vec3(0.54f, 0.89f, 0.63f);
    jade.specular_ = glm::vec3(0.316228f, 0.316228f, 0.316228f);
    jade.shininess_ = 12.8f;
    jade.alpha_ = 0.95f;

    // 黑曜石（Obsidian）
    Material obsidian;
    obsidian.ambient_ = glm::vec3(0.05375f, 0.05f, 0.06625f);
    obsidian.diffsue_ = glm::vec3(0.18275f, 0.17f, 0.22525f);
    obsidian.specular_ = glm::vec3(0.332741f, 0.328634f, 0.346435f);
    obsidian.shininess_ = 38.4f;
	obsidian.alpha_ = 0.95f;

    // 珍珠（Pearl）
    Material pearl;
    pearl.ambient_ = glm::vec3(0.25f, 0.20725f, 0.20725f);
    pearl.diffsue_ = glm::vec3(1.0f, 0.829f, 0.829f);
    pearl.specular_ = glm::vec3(0.296648f, 0.296648f, 0.296648f);
    pearl.shininess_ = 11.264f;
	pearl.alpha_ = 0.922f;

    // 红宝石（Ruby）
    Material ruby;
    ruby.ambient_ = glm::vec3(0.1745f, 0.01175f, 0.01175f);
    ruby.diffsue_ = glm::vec3(0.61424f, 0.04136f, 0.04136f);
    ruby.specular_ = glm::vec3(0.727811f, 0.626959f, 0.626959f);
    ruby.shininess_ = 76.8f;
	ruby.alpha_ = 0.55f;

    // 绿松石（Turquoise）
    Material turquoise;
    turquoise.ambient_ = glm::vec3(0.1f, 0.18725f, 0.1745f);
    turquoise.diffsue_ = glm::vec3(0.396f, 0.74151f, 0.69102f);
    turquoise.specular_ = glm::vec3(0.297254f, 0.30829f, 0.306678f);
    turquoise.shininess_ = 12.8f;
    turquoise.alpha_ = 0.8f;
    // 黄铜（Brass）
    Material brass;
    brass.ambient_ = glm::vec3(0.329412f, 0.223529f, 0.027451f);
    brass.diffsue_ = glm::vec3(0.780392f, 0.568627f, 0.113725f);
    brass.specular_ = glm::vec3(0.992157f, 0.941176f, 0.807843f);
    brass.shininess_ = 27.8974f;
    brass.alpha_ = 1.0f;

    // 青铜（Bronze）
    Material bronze;
    bronze.ambient_ = glm::vec3(0.2125f, 0.1275f, 0.054f);
    bronze.diffsue_ = glm::vec3(0.714f, 0.4284f, 0.18144f);
    bronze.specular_ = glm::vec3(0.393548f, 0.271906f, 0.166721f);
    bronze.shininess_ = 25.6f;
    bronze.alpha_ = 1.0f;
    // 铬合金（Chrome）
    Material chrome;
    chrome.ambient_ = glm::vec3(0.25f, 0.25f, 0.25f);
    chrome.diffsue_ = glm::vec3(0.4f, 0.4f, 0.4f);
    chrome.specular_ = glm::vec3(0.774597f, 0.774597f, 0.774597f);
    chrome.shininess_ = 76.8f;
    chrome.alpha_ = 1.0f;
    // 铜（Copper）
    Material copper;
    copper.ambient_ = glm::vec3(0.19125f, 0.0735f, 0.0225f);
    copper.diffsue_ = glm::vec3(0.7038f, 0.27048f, 0.0828f);
    copper.specular_ = glm::vec3(0.256777f, 0.137622f, 0.086014f);
    copper.shininess_ = 12.8f;
    copper.alpha_ = 1.0f;
    // 金（Gold）
    Material gold;
    gold.ambient_ = glm::vec3(0.24725f, 0.1995f, 0.0745f);
    gold.diffsue_ = glm::vec3(0.75164f, 0.60648f, 0.22648f);
    gold.specular_ = glm::vec3(0.628281f, 0.555802f, 0.366065f);
    gold.shininess_ = 51.2f;
    gold.alpha_ = 1.0f;
    // 银（Silver）
    Material silver;
    silver.ambient_ = glm::vec3(0.19225f, 0.19225f, 0.19225f);
    silver.diffsue_ = glm::vec3(0.50754f, 0.50754f, 0.50754f);
    silver.specular_ = glm::vec3(0.508273f, 0.508273f, 0.508273f);
    silver.shininess_ = 51.2f;
    silver.alpha_ = 1.0f;
    // ==============================
    // 标准材质（塑料 / 橡胶）
    // ==============================

    // 黑色塑料
    Material black_plastic;
    black_plastic.ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
    black_plastic.diffsue_ = glm::vec3(0.01f, 0.01f, 0.01f);
    black_plastic.specular_ = glm::vec3(0.50f, 0.50f, 0.50f);
    black_plastic.shininess_ = 32.0f;
    black_plastic.alpha_ = 1.0f;
    // 青色塑料
    Material cyan_plastic;
    cyan_plastic.ambient_ = glm::vec3(0.0f, 0.10f, 0.06f);
    cyan_plastic.diffsue_ = glm::vec3(0.0f, 0.51f, 0.51f);
    cyan_plastic.specular_ = glm::vec3(0.50f, 0.50f, 0.50f);
    cyan_plastic.shininess_ = 32.0f;
    cyan_plastic.alpha_ = 1.0f;
    // 绿色塑料
    Material green_plastic;
    green_plastic.ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
    green_plastic.diffsue_ = glm::vec3(0.10f, 0.35f, 0.10f);
    green_plastic.specular_ = glm::vec3(0.45f, 0.55f, 0.45f);
    green_plastic.shininess_ = 32.0f;
    green_plastic.alpha_ = 1.0f;
    // 红色塑料
    Material red_plastic;
    red_plastic.ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
    red_plastic.diffsue_ = glm::vec3(0.50f, 0.0f, 0.0f);
    red_plastic.specular_ = glm::vec3(0.70f, 0.60f, 0.60f);
    red_plastic.shininess_ = 32.0f;
    red_plastic.alpha_ = 1.0f;
    // 白色塑料
    Material white_plastic;
    white_plastic.ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
    white_plastic.diffsue_ = glm::vec3(0.55f, 0.55f, 0.55f);
    white_plastic.specular_ = glm::vec3(0.70f, 0.70f, 0.70f);
    white_plastic.shininess_ = 32.0f;
    white_plastic.alpha_ = 1.0f;
    // 黄色塑料
    Material yellow_plastic;
    yellow_plastic.ambient_ = glm::vec3(0.0f, 0.0f, 0.0f);
    yellow_plastic.diffsue_ = glm::vec3(0.50f, 0.50f, 0.0f);
    yellow_plastic.specular_ = glm::vec3(0.60f, 0.60f, 0.50f);
    yellow_plastic.shininess_ = 32.0f;
    yellow_plastic.alpha_ = 1.0f;
    // 黑色橡胶
    Material black_rubber;
    black_rubber.ambient_ = glm::vec3(0.02f, 0.02f, 0.02f);
    black_rubber.diffsue_ = glm::vec3(0.01f, 0.01f, 0.01f);
    black_rubber.specular_ = glm::vec3(0.40f, 0.40f, 0.40f);
    black_rubber.shininess_ = 10.0f;
    black_rubber.alpha_ = 1.0f;
    // 青色橡胶
    Material cyan_rubber;
    cyan_rubber.ambient_ = glm::vec3(0.0f, 0.05f, 0.05f);
    cyan_rubber.diffsue_ = glm::vec3(0.4f, 0.5f, 0.5f);
    cyan_rubber.specular_ = glm::vec3(0.04f, 0.70f, 0.70f);
    cyan_rubber.shininess_ = 10.0f;
    cyan_rubber.alpha_ = 1.0f;
    // 绿色橡胶
    Material green_rubber;
    green_rubber.ambient_ = glm::vec3(0.0f, 0.05f, 0.0f);
    green_rubber.diffsue_ = glm::vec3(0.4f, 0.5f, 0.4f);
    green_rubber.specular_ = glm::vec3(0.04f, 0.70f, 0.04f);
    green_rubber.shininess_ = 10.0f;
    green_rubber.alpha_ = 1.0f;
    // 红色橡胶
    Material red_rubber;
    red_rubber.ambient_ = glm::vec3(0.05f, 0.0f, 0.0f);
    red_rubber.diffsue_ = glm::vec3(0.5f, 0.4f, 0.4f);
    red_rubber.specular_ = glm::vec3(0.70f, 0.04f, 0.04f);
    red_rubber.shininess_ = 10.0f;
    red_rubber.alpha_ = 1.0f;
    // 白色橡胶
    Material white_rubber;
    white_rubber.ambient_ = glm::vec3(0.05f, 0.05f, 0.05f);
    white_rubber.diffsue_ = glm::vec3(0.5f, 0.5f, 0.5f);
    white_rubber.specular_ = glm::vec3(0.70f, 0.70f, 0.70f);
    white_rubber.shininess_ = 10.0f;
    white_rubber.alpha_ = 1.0f;
    // 黄色橡胶
    Material yellow_rubber;
    yellow_rubber.ambient_ = glm::vec3(0.05f, 0.05f, 0.0f);
    yellow_rubber.diffsue_ = glm::vec3(0.5f, 0.5f, 0.4f);
    yellow_rubber.specular_ = glm::vec3(0.70f, 0.70f, 0.04f);
    yellow_rubber.shininess_ = 10.0f;
    yellow_rubber.alpha_ = 1.0f;
    // 将所有条目写入材质表（key -> Material）
    map_material_.insert(std::make_pair(TR("emerald"), emerald));
    map_material_.insert(std::make_pair(TR("jade"), jade));
    map_material_.insert(std::make_pair(TR("obsidian"), obsidian));
    map_material_.insert(std::make_pair(TR("pearl"), pearl));
    map_material_.insert(std::make_pair(TR("ruby"), ruby));
    map_material_.insert(std::make_pair(TR("turquoise"), turquoise));
    map_material_.insert(std::make_pair(TR("brass"), brass));
    map_material_.insert(std::make_pair(TR("bronze"), bronze));
    map_material_.insert(std::make_pair(TR("chrome"), chrome));
    map_material_.insert(std::make_pair(TR("copper"), copper));
    map_material_.insert(std::make_pair(TR("gold"), gold));
    map_material_.insert(std::make_pair(TR("silver"), silver));

    map_material_.insert(std::make_pair("black_plastic", black_plastic));
    map_material_.insert(std::make_pair("cyan_plastic", cyan_plastic));
    map_material_.insert(std::make_pair("green_plastic", green_plastic));
    map_material_.insert(std::make_pair("red_plastic", red_plastic));
    map_material_.insert(std::make_pair("white_plastic", white_plastic));
    map_material_.insert(std::make_pair("yellow_plastic", yellow_plastic));

    map_material_.insert(std::make_pair("black_rubber", black_rubber));
    map_material_.insert(std::make_pair("cyan_rubber", cyan_rubber));
    map_material_.insert(std::make_pair("green_rubber", green_rubber));
    map_material_.insert(std::make_pair("red_rubber", red_rubber));
    map_material_.insert(std::make_pair("white_rubber", white_rubber));
    map_material_.insert(std::make_pair("yellow_rubber", yellow_rubber));
}
Utils::Material Utils::MaterialAttrib::getMaterial(const QString& name)
{
    return map_material_[name];
}


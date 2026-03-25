#include "Translator.h"
#include <fstream>
#include <sstream>
#include <algorithm>


Translator& Translator::getInstance()
{
	static Translator instance;
	instance.translator_map_["cube"] = tr("cube");
	instance.translator_map_["ring"] = tr("ring");
	instance.translator_map_["sky"] = tr("sky");
	instance.translator_map_["model"] = tr("model");
	instance.translator_map_["clear_sky"] = tr("clear_sky");
	instance.translator_map_["cylinder"] = tr("cylinder");
	instance.translator_map_["grid"] = tr("grid");
	instance.translator_map_["Planet"] = tr("Planet"); //行星

	instance.translator_map_["Move"] = tr("Move");	//移动
	instance.translator_map_["Rotate"] = tr("Rotate");
	instance.translator_map_["Scale"] = tr("Scale");
	instance.translator_map_["Select"] = tr("Select");//选择
	instance.translator_map_["OpenGL"] = tr("OpenGL");//OpenGL

	instance.translator_map_["Inversion"] = tr("Inversion");//反色
	instance.translator_map_["Grayscale"] = tr("Grayscale");//灰度
	instance.translator_map_["Sharpen"] = tr("Sharpen");//锐化
	instance.translator_map_["Blur"] = tr("Blur");	//模糊
	instance.translator_map_["Detection"] = tr("Detection");//检测
	instance.translator_map_["Blasting"] = tr("Blasting");	//爆破

	//右上角集合设置
	instance.translator_map_["SecenCollection"] = tr("SecenCollection");//场景集合
	instance.translator_map_["Collection"] = tr("Collection"); //集合
	instance.translator_map_["Camera"] = tr("Camera"); //相机
	instance.translator_map_["Light"] = tr("Light"); //灯光
	//右下角属性设置
	instance.translator_map_["Tool"] = tr("Tool"); //工具
	instance.translator_map_["Render"] = tr("Render"); //渲染
	instance.translator_map_["WorldSpace"] = tr("WorldSpace"); //世界空间
	instance.translator_map_["Material"] = tr("Material"); //材质
	instance.translator_map_["Object"] = tr("Object"); //物体
	instance.translator_map_["Output"] = tr("Output"); //输出

	// --- 宝石类（Gemstones） ---
	instance.translator_map_["emerald"] = tr("emerald");   // 翠（祖母绿）
	instance.translator_map_["jade"] = tr("jade");         // 玉
	instance.translator_map_["obsidian"] = tr("obsidian"); // 黑曜石
	instance.translator_map_["pearl"] = tr("pearl");       // 珍珠
	instance.translator_map_["ruby"] = tr("ruby");         // 红宝石
	instance.translator_map_["turquoise"] = tr("turquoise"); // 绿松石

	// --- 金属类（Metals） ---
	instance.translator_map_["brass"] = tr("brass");   // 黄铜
	instance.translator_map_["bronze"] = tr("bronze"); // 青铜
	instance.translator_map_["chrome"] = tr("chrome"); // 铬合金
	instance.translator_map_["copper"] = tr("copper"); // 铜
	instance.translator_map_["gold"] = tr("gold");     // 金
	instance.translator_map_["silver"] = tr("silver"); // 银

	// --- 塑料类（Plastics） ---
	instance.translator_map_["black_plastic"] = tr("black_plastic");   // 黑色塑料
	instance.translator_map_["cyan_plastic"] = tr("cyan_plastic");     // 青色塑料
	instance.translator_map_["green_plastic"] = tr("green_plastic");   // 绿色塑料
	instance.translator_map_["red_plastic"] = tr("red_plastic");       // 红色塑料
	instance.translator_map_["white_plastic"] = tr("white_plastic");   // 白色塑料
	instance.translator_map_["yellow_plastic"] = tr("yellow_plastic"); // 黄色塑料

	// --- 橡胶类（Rubbers） ---
	instance.translator_map_["black_rubber"] = tr("black_rubber");   // 黑色橡胶
	instance.translator_map_["cyan_rubber"] = tr("cyan_rubber");     // 青色橡胶
	instance.translator_map_["green_rubber"] = tr("green_rubber");   // 绿色橡胶
	instance.translator_map_["red_rubber"] = tr("red_rubber");       // 红色橡胶
	instance.translator_map_["white_rubber"] = tr("white_rubber");   // 白色橡胶
	instance.translator_map_["yellow_rubber"] = tr("yellow_rubber"); // 黄色橡胶

	//光照模型
	instance.translator_map_["phone"] = tr("phone"); //  phone 模型
	instance.translator_map_["blinn"] = tr("blinn"); // blinn 模型
	instance.translator_map_["floor"] = tr("floor"); // 地板
	
	//渲染类型
	instance.translator_map_["single_render"] = tr("single_render");	// 单面遮光
	instance.translator_map_["double_render"] = tr("double_render");	// 双面遮光
	instance.translator_map_["discard_render"] = tr("discard_render");	// 移除渲染

	return  instance;
}
QString Translator::Tr(const QString& value)
{
	return translator_map_[value];
}

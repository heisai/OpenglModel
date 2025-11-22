#include "Translator.h"
#include <fstream>
#include <sstream>
#include <algorithm>

Translator::Translator()
{
	addTranslator("Cube"); //立方体
	addTranslator("Render");//渲染
	addTranslator("Clear");//清除
	addTranslator("SkyBox");//天空盒
	translator_map_["123"] = tr("q1234");
	translator_map_["1"] = tr("q1");
	translator_map_["2"] = tr("q3");
	translator_map_["1"] = tr("q7");
	translator_map_["1"] = tr("q8");
	translator_map_["1"] = tr("q5");
	translator_map_["123"] = tr("nnnn");

}
void Translator::addTranslator(QString key)
{

	translator_map_["123"] = tr("q1234");
	translator_map_["1"] = tr("q1");
	translator_map_["2"] = tr("q3");
	translator_map_["1"] = tr("q7");
	translator_map_["1"] = tr("q8");
	translator_map_["1"] = tr("q5");
	translator_map_["1"] = tr(key.toStdString().c_str());
	translator_map_["123"] = tr("qweqwewqe");
}

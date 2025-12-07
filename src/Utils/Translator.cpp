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

	instance.translator_map_["Move"] = tr("Move");	//ÒÆ¶¯
	instance.translator_map_["Rotate"] = tr("Rotate");
	instance.translator_map_["Scale"] = tr("Scale");
	instance.translator_map_["Select"] = tr("Select");
	instance.translator_map_["OpenGL"] = tr("OpenGL");

	instance.translator_map_["Inversion"] = tr("Inversion");
	instance.translator_map_["Grayscale"] = tr("Grayscale");
	instance.translator_map_["Sharpen"] = tr("Sharpen");
	instance.translator_map_["Blur"] = tr("Blur");
	instance.translator_map_["Detection"] = tr("Detection");

	return  instance;
}
QString Translator::Tr(const QString& value)
{
	return translator_map_[value];
}

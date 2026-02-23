
#include "./src/OpenglWidget/mainwindow.h"
#include <QApplication>
#include <QFont>
#include <QSurfaceFormat>
#include <QTranslator>
#include "src/Utils/Translator.h"
#include<QDir>

int main(int argc, char *argv[])
{


    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);


	



    //qputenv("QT_SCALE_FACTOR", "1.25");
	// 配置 OpenGL 全局格式
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setSamples(8);             
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	QSurfaceFormat::setDefaultFormat(format);  // 应用到所有 QOpenGLWidget
    QApplication a(argc, argv);
    
	QTranslator appTranslator;



	
	InitLogging();
	std::string translationDir = fs::current_path().parent_path().string();
	std::cout << "上级目录: " << translationDir << std::endl;
	if (appTranslator.load("translations/zh_CN.qm", QString::fromStdString(translationDir)))
	{ 
		a.installTranslator(&appTranslator);
		LogInfo("Successfully loaded translation file: {}", appTranslator.filePath().toStdString());
	}
	else
	{
		LogInfo("Failed to load translation file from: {}/translations/zh_CN.qm", translationDir);
	}
	QFont font("Microsoft YaHei", 10); // 设置字体为 微软雅黑，字号为 10
    MainWindow w;
    w.show();
	w.initCollectionEngine();
    QFont f;
	a.setFont(font);
    return a.exec();
}

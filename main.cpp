
#include "./src/OpenglWidget/mainwindow.h"
#include <QApplication>
#include <QFont>
#include <QSurfaceFormat>
#include <QTranslator>
#include "src/Utils/Translator.h"


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
	QString qmFilename = "E:\\OPenglProduct\\build\\translations\\zh_CN.qm"; // 请根据你的实际文件命名调整
	if (appTranslator.load(qmFilename)) { // 这里假设QM文件在资源的/translations目录下
		a.installTranslator(&appTranslator);
		qDebug() << "Successfully loaded translation file:" << qmFilename;
	}
	else {
		qDebug() << "Failed to load translation file:" << qmFilename << "Falling back to default language.";
		// 可以在这里加载一个默认语言（例如英语）的QM文件
	}
	qDebug() << TR("Cube");

	QFont font("Microsoft YaHei", 10); // 设置字体为 微软雅黑，字号为 10
	
	InitLogging();
    MainWindow w;
    w.show();
	//w.showGridEngine();
    QFont f;
	a.setFont(font);
    return a.exec();
}

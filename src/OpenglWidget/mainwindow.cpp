#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

void MainWindow::InitUI()
{
    m_PtrManageEngine = std::make_shared<ManageEngine>();
    ui->openGLWidget->BindManageEngine(m_PtrManageEngine);
    this->setWindowTitle(TR("OpenGL"));
    this->setMinimumSize(QSize(1240, 780));

    addEngineMenu();
    addRenderMenu();


    QAction* clear_model_action = new QAction(TR("model"), ui->clear_menu);
    clear_model_action->setObjectName("clear_model");
    QAction* clear_sky_action = new QAction(TR("sky"), ui->clear_menu);
    clear_sky_action->setObjectName("clear_sky");
    ui->clear_menu->addAction(clear_model_action);
    ui->clear_menu->addAction(clear_sky_action);
    
    
}

void MainWindow::InitConnect()
{
	for (auto action : this->findChildren<QAction*>())
	{
		connect(action, &QAction::triggered, this, &MainWindow::createModel,Qt::UniqueConnection);
	}
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addEngineMenu()
{
	QAction* ring_action = new QAction(TR("ring"), ui->model_menu);
	ring_action->setObjectName("ring");

	QAction* cube_action = new QAction(TR("cube"), ui->model_menu);
	cube_action->setObjectName("cube_action");

	QAction* cylinder_action = new QAction(TR("cylinder"), ui->model_menu);
	cylinder_action->setObjectName("cylinder_action");

	ui->model_menu->addAction(ring_action);
	ui->model_menu->addAction(cube_action);
	ui->model_menu->addAction(cylinder_action);
}

void MainWindow::addRenderMenu()
{
	QAction* sky_action = new QAction(TR("sky"), ui->render_menu);
	sky_action->setObjectName("sky");

	QAction* inversion_action = new QAction(TR("Inversion"), ui->render_menu);
    inversion_action->setObjectName("Inversion");

	QAction* grayscale_action = new QAction(TR("Grayscale"), ui->render_menu);
    grayscale_action->setObjectName("Grayscale");

	QAction* sharpen_action = new QAction(TR("Sharpen"), ui->render_menu);
    sharpen_action->setObjectName("Sharpen");

	QAction* blur_action = new QAction(TR("Blur"), ui->render_menu);
    blur_action->setObjectName("Blur");

	QAction* detection_action = new QAction(TR("Detection"), ui->render_menu);
    detection_action->setObjectName("Detection");

	ui->render_menu->addAction(sky_action);
	ui->render_menu->addAction(inversion_action);
    ui->render_menu->addAction(grayscale_action);
    ui->render_menu->addAction(sharpen_action);
    ui->render_menu->addAction(blur_action);
    ui->render_menu->addAction(detection_action);

}

void MainWindow::createModel(bool checked)
{
	QString objname = sender()->objectName();
	if (objname == "ring")
	{
        CreatEngine(true, OperatorAction::CreatCyliner);
	}
	else if (objname == "cube_action")
	{
        CreatEngine(true, OperatorAction::CreatCube);
	}
	else if (objname == "cylinder_action")
	{
        CreatEngine(true, OperatorAction::CreatTourse);
	}
    else if (objname == "sky")
    {
        CreatEngine(true, OperatorAction::ClearSkyBox);
    }
    else if (objname == "clear_model")
    {
        CreatEngine(false, OperatorAction::ClearCreatModel);
    }
    else if (objname == "clear_sky")
    {
        CreatEngine(false, OperatorAction::ClearSkyBox);
    }
	else if (objname == "Inversion")
	{
		CreatEngine(true, OperatorAction::RenderInversion);
	}
	else if (objname == "Grayscale")
	{
		CreatEngine(true, OperatorAction::RenderGrayscale);
	}
	else if (objname == "Sharpen")
	{
		CreatEngine(true, OperatorAction::RenderSharpen);
	}
	else if (objname == "Blur")
	{
		CreatEngine(true, OperatorAction::RenderBlur);
	}
	else if (objname == "Detection")
	{
		CreatEngine(true, OperatorAction::RenderDetection);
	}
    LogInfo("create model type:{}", objname.toStdString());
    qDebug() << objname;
}

void MainWindow::CreatEngine(bool checked, OperatorAction type)
{
    if(checked)
    {
       
        m_PtrManageEngine->createModel(type);
        ui->openGLWidget->makeCurrent();
        m_PtrManageEngine->initializeGl();
        ui->openGLWidget->update();
        ui->openGLWidget->doneCurrent();
    }
    else
    {
         m_PtrManageEngine->removeModel(type);
    }
}

void MainWindow::ChangeLightColorSlot(QColor color)
{
    //m_PtrManageEngine->ChangeLightColor(color.red()/255.0, color.green()/255.0, color.blue()/255.0);
}



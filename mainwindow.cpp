#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    this->setWindowTitle("OpenGLDemon");

    QAction* ring_action = new QAction("ring", ui->model_menu);
    ring_action->setObjectName("ring");
    QAction* cube_action = new QAction("cube", ui->model_menu);
    cube_action->setObjectName("cube_action");
    QAction* cylinder_action = new QAction("cylinder", ui->model_menu);
    cylinder_action->setObjectName("cylinder_action");
	ui->model_menu->addAction(ring_action);
    ui->model_menu->addAction(cube_action);
	ui->model_menu->addAction(cylinder_action);

    QAction* sky_action = new QAction("sky", ui->render_menu);
    sky_action->setObjectName("sky");
    ui->render_menu->addAction(sky_action);

    QAction* clear_model_action = new QAction("model", ui->clear_menu);
    clear_model_action->setObjectName("clear_model");
    QAction* clear_sky_action = new QAction("sky", ui->clear_menu);
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


void MainWindow::createModel(bool checked)
{
	QString objname = sender()->objectName();
	if (objname == "ring")
	{
        CreatEngine(true, EM_CYLINDERENGINE);
	}
	else if (objname == "cube_action")
	{
        CreatEngine(true, EM_CUBEENGINE);
	}
	else if (objname == "cylinder_action")
	{
        CreatEngine(true, EM_TOURSENGINE);
	}
    else if (objname == "sky")
    {
        CreatEngine(true, EM_SKYBOXENGINE);
    }
    else if (objname == "clear_model")
    {
        CreatEngine(false, EM_GRIDENGINE);
    }
    else if (objname == "clear_sky")
    {
        CreatEngine(false, EM_SKYBOXENGINE);
    }
}

void MainWindow::CreatEngine(bool checked, EngineType type)
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



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

    
    
}

void MainWindow::InitConnect()
{
    connect(ui->openGLWidget, &CustomOpenglWidget::CheckBoxType, m_PtrManageEngine.get(), &ManageEngine::checkBoxTypeSlot);

	for (auto action : ui->model_menu->findChildren<QAction*>())
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
         // m_PtrManageEngine->RemoveEngine(type);
    }
}

void MainWindow::ChangeLightColorSlot(QColor color)
{
    //m_PtrManageEngine->ChangeLightColor(color.red()/255.0, color.green()/255.0, color.blue()/255.0);
}



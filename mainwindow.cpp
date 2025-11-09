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
    
    
}

void MainWindow::InitConnect()
{
    connect(ui->widget,&ParaConfigWidget::CheckBoxSig,this,&MainWindow::CreatEngine);
    //connect(ui->widget, &ParaConfigWidget::ChangeLightColorSig, this, &MainWindow::ChangeLightColorSlot);
    connect(ui->openGLWidget, &CustomOpenglWidget::CheckBoxType, m_PtrManageEngine.get(), &ManageEngine::checkBoxTypeSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
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



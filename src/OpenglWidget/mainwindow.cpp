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
	addClearMenu();

	
}

void MainWindow::InitConnect()
{
	for (auto action : this->findChildren<QAction*>())
	{
		connect(action, &QAction::triggered, this, &MainWindow::createModel,Qt::UniqueConnection);
	}

}

void MainWindow::showGridEngine()
{
	m_PtrManageEngine->createModel(OperatorAction::CreatGrid);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addEngineMenu()
{
	QAction* ring_action = new QAction(TR("ring"), ui->model_menu);
	ring_action->setObjectName(QString::number(static_cast<int>(OperatorAction::CreatTourse)));

	QAction* cube_action = new QAction(TR("cube"), ui->model_menu);
	cube_action->setObjectName(QString::number(static_cast<int>(OperatorAction::CreatCube)));

	QAction* cylinder_action = new QAction(TR("cylinder"), ui->model_menu);
	cylinder_action->setObjectName(QString::number(static_cast<int>(OperatorAction::CreatCyliner)));

	QAction* grid_action = new QAction(TR("grid"), ui->model_menu);
	grid_action->setObjectName(QString::number(static_cast<int>(OperatorAction::CreatGrid)));

	ui->model_menu->addAction(ring_action);
	ui->model_menu->addAction(cube_action);
	ui->model_menu->addAction(cylinder_action);
	ui->model_menu->addAction(grid_action);
}

void MainWindow::addRenderMenu()
{

	QAction* sky_action = new QAction(TR("sky"), ui->render_menu);
	sky_action->setObjectName(QString::number(static_cast<int>(OperatorAction::RenderSky)));

	QAction* inversion_action = new QAction(TR("Inversion"), ui->render_menu);
    inversion_action->setObjectName(QString::number(static_cast<int>(OperatorAction::RenderInversion)));

	QAction* grayscale_action = new QAction(TR("Grayscale"), ui->render_menu);
    grayscale_action->setObjectName(QString::number(static_cast<int>(OperatorAction::RenderGrayscale)));

	QAction* sharpen_action = new QAction(TR("Sharpen"), ui->render_menu);
    sharpen_action->setObjectName(QString::number(static_cast<int>(OperatorAction::RenderSharpen)));

	QAction* blur_action = new QAction(TR("Blur"), ui->render_menu);
    blur_action->setObjectName(QString::number(static_cast<int>(OperatorAction::RenderBlur)));

	QAction* detection_action = new QAction(TR("Detection"), ui->render_menu);
    detection_action->setObjectName(QString::number(static_cast<int>(OperatorAction::RenderDetection)));

	QAction* blasting_action = new QAction(TR("Blasting"), ui->render_menu);
	blasting_action->setObjectName(QString::number(static_cast<int>(OperatorAction::RenderBlasting)));

	ui->render_menu->addAction(sky_action);
	ui->render_menu->addAction(inversion_action);
    ui->render_menu->addAction(grayscale_action);
    ui->render_menu->addAction(sharpen_action);
    ui->render_menu->addAction(blur_action);
    ui->render_menu->addAction(detection_action);
	ui->render_menu->addAction(blasting_action);

}

void MainWindow::addClearMenu()
{
	QAction* clear_model_action = new QAction(TR("model"), ui->clear_menu);
	clear_model_action->setObjectName(QString::number(static_cast<int>(OperatorAction::ClearCreatModel)));
	QAction* clear_sky_action = new QAction(TR("sky"), ui->clear_menu);
	clear_sky_action->setObjectName(QString::number(static_cast<int>(OperatorAction::ClearSkyBox)));

	QAction* clear_blasting_action = new QAction(TR("Blasting"), ui->render_menu);
	clear_blasting_action->setObjectName(QString::number(static_cast<int>(OperatorAction::RenderBlasting)));

	ui->clear_menu->addAction(clear_model_action);
	ui->clear_menu->addAction(clear_sky_action);
	ui->clear_menu->addAction(clear_blasting_action);
}

void MainWindow::createModel(bool checked)
{
	OperatorAction operator_action = static_cast<OperatorAction>(sender()->objectName().toInt());
	 if (operator_action >= OperatorAction::ClearSkyBox && operator_action <= OperatorAction::ClearRender)
	 {
		 CreatEngine(false, operator_action);
	 }
	 else
	 {
		 CreatEngine(true, operator_action);
	 }
    LogInfo("create model type:{}", static_cast<int>(operator_action));
}

void MainWindow::CreatEngine(bool create_model, OperatorAction type)
{
    if(create_model)
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



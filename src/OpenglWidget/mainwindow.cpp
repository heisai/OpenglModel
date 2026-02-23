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
    manage_engine_moudle_ = std::make_shared<ManageEngine>();
    ui->openGLWidget->BindManageEngine(manage_engine_moudle_);
    this->setWindowTitle(TR("OpenGL"));
    this->setMinimumSize(QSize(1240, 780));


	initMenuBar();
	initCenterWidget();

	
}

void MainWindow::InitConnect()
{
	for (auto action : this->findChildren<QAction*>())
	{
		connect(action, &QAction::triggered, this, &MainWindow::operatorModel, Qt::UniqueConnection);
	}

	//更新模型集合树Item 选中状态
	connect(manage_engine_moudle_.get(), &ManageEngine::selectModelSignals, [this](const QString model_uuid) {
		ui->widget->updateTreewidgetItem(model_uuid);
		});
	//更新属性配置信息
	connect(ui->widget, &ParaConfigWidget::sigUpdatePropertyToEngine, [this](const Utils::Material& material) {
		manage_engine_moudle_->setPropertyData(material);
		});

}

void MainWindow::initCollectionEngine()
{
	//创建网格
	//manage_engine_moudle_->createModel(OperatorAction::CreatGrid);
	//创建坐标轴
	manage_engine_moudle_->createModel(OperatorAction::CreatAxis);


	
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initMenuBar()
{
	addEngineMenu();
	addRenderMenu();
	addClearMenu();
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

	QAction* planet_action = new QAction(TR("Planet"), ui->model_menu);
	planet_action->setObjectName(QString::number(static_cast<int>(OperatorAction::CreatPlanet)));

	ui->model_menu->addAction(ring_action);
	ui->model_menu->addAction(cube_action);
	ui->model_menu->addAction(cylinder_action);
	ui->model_menu->addAction(grid_action);
	ui->model_menu->addAction(planet_action);
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
	clear_blasting_action->setObjectName(QString::number(static_cast<int>(OperatorAction::ClearBlasting)));

	QAction* clear_render_action = new QAction(TR("Render"), ui->render_menu);
	clear_render_action->setObjectName(QString::number(static_cast<int>(OperatorAction::ClearRender)));

	ui->clear_menu->addAction(clear_model_action);
	ui->clear_menu->addAction(clear_sky_action);
	ui->clear_menu->addAction(clear_blasting_action);
	ui->clear_menu->addAction(clear_render_action);
}

void MainWindow::initCenterWidget()
{
	//this->horizontalLayout;
	QSplitter* splitterHorizontal = new QSplitter(Qt::Horizontal, nullptr);
	splitterHorizontal->addWidget(ui->openGLWidget);
	splitterHorizontal->addWidget(ui->widget);
	ui->horizontalLayout->addWidget(splitterHorizontal);
}

void MainWindow::operatorModel(bool checked)
{
	OperatorAction operator_action = static_cast<OperatorAction>(sender()->objectName().toInt());
	 if (operator_action >= OperatorAction::ClearSkyBox && operator_action <= OperatorAction::ClearBlasting)
	 {
		 removeModel(operator_action);
	 }
	 else
	 {
		 createModel(operator_action);
	 }
   
}

void MainWindow::createModel(OperatorAction type)
{
	ui->openGLWidget->makeCurrent();
	QString model_uuid = manage_engine_moudle_->createModel(type);

	manage_engine_moudle_->initializeGl();
	ui->openGLWidget->update();
	ui->openGLWidget->doneCurrent();
	LogInfo("create model type:{}", static_cast<int>(type));

	ui->widget->addCollectionToTreewidget(model_uuid);
}

void MainWindow::removeModel(OperatorAction type)
{
	QString model_uuid  = manage_engine_moudle_->removeModel(type);
	ui->widget->removeCollectionFromTreewidget(model_uuid);
}


void MainWindow::ChangeLightColorSlot(QColor color)
{
    //manage_engine_moudle_->ChangeLightColor(color.red()/255.0, color.green()/255.0, color.blue()/255.0);
}



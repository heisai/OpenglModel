#include "ParaConfigWidget.h"
#include"ui_ParaConfigWidget.h"
ParaConfigWidget::ParaConfigWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ParaConfigWidget)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

ParaConfigWidget::~ParaConfigWidget()
{
    delete ui;
}

void ParaConfigWidget::InitUI()
{
	//集合界面
	initCollectionWidget();
	//属性界面
	initPropertiesConfigWidget();
}

void ParaConfigWidget::InitConnect()
{

}

void ParaConfigWidget::initCollectionWidget()
{
	initTreewidget();
}

void ParaConfigWidget::initTreewidget()
{
	ui->ModelWidget->setFixedHeight(320);
	QVBoxLayout* verticalLayout = new QVBoxLayout;
	verticalLayout->setContentsMargins(0, 0, 0, 0);
	ui->ModelWidget->setLayout(verticalLayout);

	collection_treewidget_ = new QTreeWidget;
	verticalLayout->addWidget(collection_treewidget_);
	collection_treewidget_->setHeaderLabels({ TR("SecenCollection")});
	collection_treewidget_->setColumnCount(1);
	treewidgetitem_ = new QTreeWidgetItem(collection_treewidget_);
	treewidgetitem_->setText(0, TR("Collection"));
	treewidgetitem_->addChild(new QTreeWidgetItem({ TR("Camera") }));
	treewidgetitem_->addChild(new QTreeWidgetItem({ TR("Light") }));
	collection_treewidget_->addTopLevelItem(treewidgetitem_);

	collection_treewidget_->setFrameShape(QFrame::NoFrame);
	//collection_treewidget_->setIndentation(0);  // 设置为0完全移除缩进
}

void ParaConfigWidget::addCollectionToTreewidget(const QString& item_text)
{
	treewidgetitem_->addChild(new QTreeWidgetItem({ item_text }));
}

void ParaConfigWidget::initPropertiesConfigWidget()
{
	QHBoxLayout* hboxLayout = new QHBoxLayout;
	hboxLayout->addWidget(initToolButtons());
	hboxLayout->addWidget(initParamterConfigPage());
	hboxLayout->setContentsMargins(6, 6, 6, 6);
	ui->SceneWidget->setLayout(hboxLayout);
}

QWidget*ParaConfigWidget::initToolButtons()
{

	QPushButton* tool_btn = new QPushButton(TR("Tool"));
	QPushButton* tool_btn1 = new QPushButton(TR("Render"));
	QPushButton* tool_btn2 = new QPushButton(TR("WorldSpace"));
	QPushButton* tool_btn3 = new QPushButton(TR("Material"));
	QPushButton* tool_btn4 = new QPushButton(TR("Object"));
	QPushButton* tool_btn5 = new QPushButton(TR("Output"));
	vec_toolbtns.push_back(tool_btn);
	vec_toolbtns.push_back(tool_btn1);
	vec_toolbtns.push_back(tool_btn2);
	vec_toolbtns.push_back(tool_btn3);
	vec_toolbtns.push_back(tool_btn4);
	vec_toolbtns.push_back(tool_btn5);


	


	QVBoxLayout* verticalLayout = new QVBoxLayout;
	verticalLayout->setContentsMargins(0, 0, 0, 0);
	for (int index = 0; index < vec_toolbtns.size(); index++)
	{
		vec_toolbtns[index]->setFixedSize(QSize(35, 35));
		verticalLayout->addWidget(vec_toolbtns[index]);
	}
	QSpacerItem* spaceitem = new QSpacerItem(80, 500, QSizePolicy::Maximum, QSizePolicy::Minimum);
	verticalLayout->addSpacerItem(spaceitem);
	verticalLayout->setContentsMargins(0, 0, 0, 0);
	verticalLayout->setSpacing(6);

	QWidget* widget = new QWidget;
	widget->setLayout(verticalLayout);
	return widget;
}

QWidget* ParaConfigWidget::initParamterConfigPage()
{
	stackwidget_ = new QStackedWidget;
	for (int index = 0; index < vec_toolbtns.size(); index++)
	{
		stackwidget_->insertWidget(index, new QWidget());
	}
	return stackwidget_;

}

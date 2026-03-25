#include "ParaConfigWidget.h"
#include"ui_ParaConfigWidget.h"
ParaConfigWidget::ParaConfigWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ParaConfigWidget),
	collection_treewidget_(nullptr),
	treewidgetitem_(nullptr),
	stackwidget_(nullptr),
	property_editor_widget_(nullptr)
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
	initPropertyConfigWidget();
}

void ParaConfigWidget::InitConnect()
{
	connect(collection_treewidget_, &QTreeWidget::itemClicked, [this](QTreeWidgetItem* item, int column) {
		LogInfo("Select Item: {}", item->text(column).toStdString());
		});
}

void ParaConfigWidget::slotsTreeWidgetItemClicked(QTreeWidgetItem* item, int column)
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
	treewidgetitem_->addChild(new QTreeWidgetItem({ TR("floor") }));
	collection_treewidget_->addTopLevelItem(treewidgetitem_);

	collection_treewidget_->setFrameShape(QFrame::NoFrame);
	//collection_treewidget_->setIndentation(0);  // 设置为0完全移除缩进
	collection_treewidget_->expandAll();
}

void ParaConfigWidget::addCollectionToTreewidget(const QString& item_text)
{
	treewidgetitem_->addChild(new QTreeWidgetItem({ item_text }));
}

void ParaConfigWidget::removeCollectionFromTreewidget(const QString& item_text)
{

	QList<QTreeWidgetItem*> items = collection_treewidget_->findItems(item_text, Qt::MatchExactly| Qt::MatchRecursive);
	if (!items.isEmpty())
	{
		treewidgetitem_->removeChild(items.at(0));
	}
}

void ParaConfigWidget::updateTreewidgetItem(const QString& item_text)
{
	//清空当前节点的选中状态
	int count = treewidgetitem_->childCount();
	for (int index = 0; index < count; index++)
	{
		treewidgetitem_->child(index)->setSelected(false);
	}

	//设置选中状态
	QList<QTreeWidgetItem*>items = collection_treewidget_->findItems(item_text, Qt::MatchExactly | Qt::MatchRecursive);
	if (!items.isEmpty())
	{
		items.at(0)->setSelected(true);
	}

	//更新模型属性信息
	emit sigUpdatePropertyToEngine(property_editor_widget_->getPropertyInfo());
}
void ParaConfigWidget::initPropertyConfigWidget()
{
	stackwidget_ = new QStackedWidget;
	property_editor_widget_ = new PropertyEditorWidget(this);
	stackwidget_->insertWidget(0, property_editor_widget_);



	QHBoxLayout* hboxLayout = new QHBoxLayout;
	//hboxLayout->addWidget(initToolButtons());
	hboxLayout->addWidget(stackwidget_);
	hboxLayout->setContentsMargins(6, 6, 6, 6);
	ui->SceneWidget->setLayout(hboxLayout);

	connect(property_editor_widget_, &PropertyEditorWidget::sigUpdatePropertyInfo, [this](const Utils::Material& value)
		{
			emit sigUpdatePropertyToEngine(value);
		});
}

QWidget*ParaConfigWidget::initToolButtons()
{

	QPushButton* tool_btn = new QPushButton(TR("Tool"));
	QPushButton* tool_btn1 = new QPushButton(TR("Render"));
	QPushButton* tool_btn2 = new QPushButton(TR("WorldSpace"));
	QPushButton* tool_btn3 = new QPushButton(TR("Material"));
	QPushButton* tool_btn4 = new QPushButton(TR("Object"));
	QPushButton* tool_btn5 = new QPushButton(TR("Output"));

	vec_toolbtns.push_back(tool_btn3);
	vec_toolbtns.push_back(tool_btn);
	vec_toolbtns.push_back(tool_btn1);
	vec_toolbtns.push_back(tool_btn2);

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

void ParaConfigWidget::updatePropertyEditorWidget()
{

}

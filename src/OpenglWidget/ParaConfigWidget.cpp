#include "ParaConfigWidget.h"
#include "ui_ParaConfigWidget.h"

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
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(Qt::red));
}

void ParaConfigWidget::InitConnect()
{

}

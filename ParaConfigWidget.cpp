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
    for( auto checkbox : this->findChildren<QCheckBox*>())
    {
         connect(checkbox,&QCheckBox::clicked,this,&ParaConfigWidget::CheckBoxSlot/*,Qt::UniqueConnection*/);
    }
    // connect(ui->checkBoxGrid,&QCheckBox::clicked,this,&ParaConfigWidget::CheckBoxSlot);
    // connect(ui->checkBoxModel,&QCheckBox::clicked,this,&ParaConfigWidget::CheckBoxSlot);
    // connect(ui->checkBoxRect,&QCheckBox::clicked,this,&ParaConfigWidget::CheckBoxSlot);
    // connect(ui->checkBox,&QCheckBox::clicked,this,&ParaConfigWidget::CheckBoxSlot);

}


void ParaConfigWidget::CheckBoxSlot(bool checked)
{
    QString objname = sender()->objectName();
    if(objname == "checkBoxGrid")
    {
        CheckBoxSig(checked,EM_GRIDENGINE);
    }
    else if(objname == "checkBoxRect")
    {
        CheckBoxSig(checked,EM_CUBEENGINE);
    }
    else if(objname == "checkBoxModel")
    {
        CheckBoxSig(checked,EM_MODELENGINE);
    }
    else if(objname == "checkBoxBasicLight")
    {
        //CheckBoxSig(checked,EM_LIGHTMODELENGINE);
    }
    else if (objname == "checkBoxMix")
    {
        CheckBoxSig(checked, EM_MIXENGINE);
    }
	else if (objname == "checkBoxTours")
	{
		CheckBoxSig(checked, EM_TOURSENGINE);
	}
    else if(objname == "checkBoxCylinder")
    {
        CheckBoxSig(checked, EM_CYLINDERENGINE);
    }
}

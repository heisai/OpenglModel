#ifndef PARACONFIGWIDGET_H
#define PARACONFIGWIDGET_H
#include"./src/Utils/OpengHearder.h"
#include <QWidget>
#include<QMouseEvent>
#include<QColorDialog>
namespace Ui {
class ParaConfigWidget;
}

class ParaConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParaConfigWidget(QWidget *parent = nullptr);
    ~ParaConfigWidget();
    void InitUI();
    void InitConnect();
private slots:
    void CheckBoxSlot(bool checked);
signals:
    void CheckBoxSig(bool checked,EngineType type);
 
private:
    Ui::ParaConfigWidget *ui;
};

#endif // PARACONFIGWIDGET_H

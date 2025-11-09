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
private:
    Ui::ParaConfigWidget *ui;
};

#endif // PARACONFIGWIDGET_H

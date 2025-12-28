#ifndef PARACONFIGWIDGET_H
#define PARACONFIGWIDGET_H
#include "../Utils/OpengHearder.h"
#include <QWidget>
#include<QMouseEvent>
#include<QColorDialog>
#include<QTreeWidget>
#include<QPushButton>
#include<QStackedWidget>
#include<QHBoxLayout>
#include<QSpacerItem>
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
    //集合树界面
    void initCollectionWidget();
    void initTreewidget();
    void addCollectionToTreewidget(const QString &item_text);

    //属性设置界面
    void initPropertiesConfigWidget();
    QWidget* initToolButtons();
    QWidget *initParamterConfigPage();
private:
    Ui::ParaConfigWidget *ui;
    QTreeWidget *collection_treewidget_;
    QTreeWidgetItem* treewidgetitem_;
    std::vector<QPushButton*>vec_toolbtns;

    QStackedWidget* stackwidget_;
};

#endif // PARACONFIGWIDGET_H

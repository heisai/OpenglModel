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
#include"PropertyEditorWidget.h"
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
    void addCollectionToTreewidget(const QString& item_text);
    void  removeCollectionFromTreewidget(const QString& item_text);
    void updateTreewidgetItem(const QString& item_text);
signals:
	void sigUpdatePropertyToEngine(const Utils::Material& value);
private  slots:
    void slotsTreeWidgetItemClicked(QTreeWidgetItem* item, int column);
private:
    //集合树界面
    void initCollectionWidget();
    void initTreewidget();
    //属性设置界面
    void initPropertyConfigWidget();
    QWidget* initToolButtons();
    //更新属性编辑界面
    void updatePropertyEditorWidget();
private:
    Ui::ParaConfigWidget *ui;
    QTreeWidget *collection_treewidget_;
    QTreeWidgetItem* treewidgetitem_;
    std::vector<QPushButton*>vec_toolbtns;

    QStackedWidget* stackwidget_;
	PropertyEditorWidget* property_editor_widget_;
};

#endif // PARACONFIGWIDGET_H

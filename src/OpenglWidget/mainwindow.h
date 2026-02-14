#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"../Graphics/ManageEngine.h"
#include<QDockWidget>
#include<QSplitter>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void InitUI();
    void InitConnect();
	void initCollectionEngine();
    ~MainWindow();
private:
    //初始化菜单栏
    void initMenuBar();
    //添加引擎菜单
    void addEngineMenu();
    //添加渲染菜单
    void addRenderMenu();
	//添加清除菜单
	void addClearMenu();

    //初始化中心界面
    void initCenterWidget();
private slots:
    //操作模型（创建、删除、渲染）
    void operatorModel(bool checked);
    //创建模型
    void createModel(OperatorAction type);
    //移除模型
    void removeModel(OperatorAction type);

    void ChangeLightColorSlot(QColor color);
private:
    Ui::MainWindow *ui;
    std::shared_ptr<ManageEngine>manage_engine_moudle_;
  

   
};
#endif // MAINWINDOW_H

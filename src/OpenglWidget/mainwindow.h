#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"../Graphics/ManageEngine.h"
#include<QDockWidget>
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
	void showGridEngine();
    ~MainWindow();
private:
    //添加引擎菜单
    void addEngineMenu();
    //添加渲染菜单
    void addRenderMenu();
	//添加清除菜单
	void addClearMenu();
private slots:
    void createModel(bool checked);
    void CreatEngine(bool create_model,OperatorAction type);
    void ChangeLightColorSlot(QColor color);
private:
    Ui::MainWindow *ui;
    std::shared_ptr<ManageEngine>m_PtrManageEngine;
  

   
};
#endif // MAINWINDOW_H

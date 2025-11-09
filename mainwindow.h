#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"src/Graphics/ManageEngine.h"
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
    ~MainWindow();

private slots:
    void CreatEngine(bool checked,EngineType type);
    void ChangeLightColorSlot(QColor color);
private:
    Ui::MainWindow *ui;
    std::shared_ptr<ManageEngine>m_PtrManageEngine;
  

   
};
#endif // MAINWINDOW_H

#pragma once

#include <QWidget>

#include <QPushButton>
#include <QVBoxLayout>
#include <QString>
#include<QPainter>
#include<QDebug>
#include"../Utils/Translator.h"
class HoverToolWidget : public QWidget
{
    Q_OBJECT
public:
    enum ModelState
    {
        MoveState,      //移动状态
        RotateState,    //旋转状态
		ScaleState,      //缩放状态
		SelectState,    //选择状态
    };
    explicit HoverToolWidget(QWidget* parent = nullptr);
    ~HoverToolWidget() = default;
	void initializeWidget();
    ModelState getModelState() const;
   private slots:
       void clieckButton(bool clieck);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    QVBoxLayout* m_layout;
	std::vector<QPushButton*>buttons_;
    QSpacerItem* space_item_;
    int model_state_;
};

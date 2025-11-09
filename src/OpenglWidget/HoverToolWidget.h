#pragma once

#include <QWidget>

#include <QPushButton>
#include <QVBoxLayout>
#include <QString>
#include<QPainter>
#include<QDebug>
class HoverToolWidget : public QWidget
{
    Q_OBJECT
public:
    enum ModelState
    {
        MoveState,
        RotateState,
        ScaleState,
        SelectState,
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

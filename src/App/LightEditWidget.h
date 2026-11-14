#ifndef LIGHTEDITWIDGET_H
#define LIGHTEDITWIDGET_H

#include <QWidget>
#include <QMetaType>
#include "../Engine/Render/LightConfig.h"

Q_DECLARE_METATYPE(Lighting::LightPanelData)

class QDoubleSpinBox;
class QLabel;

namespace Ui
{
class LightEditWidget;
}

class LightEditWidget : public QWidget
{
    Q_OBJECT

  public:
    // 创建控件，以统一默认参数初始化面板并连接编辑信号。
    explicit LightEditWidget(QWidget *parent = nullptr);
    // 释放生成的界面对象，子控件由 Qt 管理。
    ~LightEditWidget();

    // 创建灯类型、模式、数值编辑控件和错误提示。
    void initUI();
    // 将类型、模式和数值编辑统一接入整份参数的提交信号。
    void initConnect();
    // 返回当前编辑草稿；未改动的数值保留引擎传入的完整精度。
    Lighting::LightPanelData panelData() const;
    // 回显引擎已接受的完整参数，并阻断控件信号以避免递归提交。
    void setPanelData(const Lighting::LightPanelData &data);
    // 显示引擎的校验错误，不修改已提交状态。
    void showValidationError(const QString &message);

  signals:
    // 用户修改任一控件后提交完整草稿，由引擎统一校验并决定是否应用。
    void sigUpdateLightPanel(const Lighting::LightPanelData &data);

  private slots:
    // 更新草稿中的类型和模式，并提交整份参数。
    void onPanelChanged();

  private:
    // 只修改实际编辑的数值字段，避免其他字段因控件舍入而被改写。
    void onValueChanged(int index, double value);
    Ui::LightEditWidget *ui;
    QDoubleSpinBox *values_[8]{};
    QLabel *error_ = nullptr;
    Lighting::LightPanelData panel_data_;
};

#endif // LIGHTEDITWIDGET_H

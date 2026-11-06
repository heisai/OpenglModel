#ifndef LIGHTEDITWIDGET_H
#define LIGHTEDITWIDGET_H

#include <QWidget>
#include "../Engine/Render/LightConfig.h"

namespace Ui {
class LightEditWidget;
}

class LightEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LightEditWidget(QWidget *parent = nullptr);
    ~LightEditWidget();

    void initUI();
    void initConnect();
    Lighting::LightType getLightType() const;
    Lighting::LightMode getLightMode() const;

signals:
    void sigUpdateLightInfo(Lighting::LightType lightType, Lighting::LightMode lightMode);

private slots:
    void onLightSettingChanged();

private:
    Ui::LightEditWidget *ui;
};

#endif // LIGHTEDITWIDGET_H

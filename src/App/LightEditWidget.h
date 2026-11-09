#ifndef LIGHTEDITWIDGET_H
#define LIGHTEDITWIDGET_H

#include <QWidget>
#include "../Engine/Render/LightConfig.h"
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
    explicit LightEditWidget(QWidget *parent = nullptr);
    ~LightEditWidget();

    void initUI();
    void initConnect();
    Lighting::LightType getLightType() const;
    Lighting::LightMode getLightMode() const;
    void setValues(Lighting::LightType type, Lighting::LightMode mode, const glm::vec3 &position,
                   const glm::vec3 &direction, float inner, float outer);

  signals:
    void sigUpdateLightInfo(Lighting::LightType lightType, Lighting::LightMode lightMode);
    void sigUpdateLightValues(const glm::vec3 &position, const glm::vec3 &direction, float inner, float outer);

  private slots:
    void onLightSettingChanged();

  private:
    void onValuesChanged();
    Ui::LightEditWidget *ui;
    QDoubleSpinBox *values_[8]{};
    QLabel *error_ = nullptr;
};

#endif // LIGHTEDITWIDGET_H

#include "LightEditWidget.h"
#include "ui_LightEditWidget.h"
#include "../Utils/Translator.h"
#include <QComboBox>

LightEditWidget::LightEditWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LightEditWidget)
{
    ui->setupUi(this);
    initUI();
    initConnect();
}

LightEditWidget::~LightEditWidget()
{
    delete ui;
}

void LightEditWidget::initUI()
{
    setWindowTitle(TR("Light settings"));
    // The .ui file places these named labels next to the opposite controls.
    ui->lightTypeLabel->setText(TR("Light mode"));
    ui->lightModeLabel->setText(TR("Light type"));

    // 枚举值保存在条目数据中，显示顺序不必与枚举值绑定。
    ui->lightTypeComboBox->addItem(TR("DirectionalLight"), static_cast<int>(Lighting::LightType::Directional));
    ui->lightTypeComboBox->addItem(TR("PointLight"), static_cast<int>(Lighting::LightType::Point));
    ui->lightTypeComboBox->addItem(TR("SpotLight"), static_cast<int>(Lighting::LightType::Spot));

    ui->lightModeComboBox->addItem(TR("SingleLight"), static_cast<int>(Lighting::LightMode::Single));
    ui->lightModeComboBox->addItem(TR("MultipleLights"), static_cast<int>(Lighting::LightMode::Multiple));
}

void LightEditWidget::initConnect()
{
    connect(ui->lightTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &LightEditWidget::onLightSettingChanged);
    connect(ui->lightModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &LightEditWidget::onLightSettingChanged);
}

Lighting::LightType LightEditWidget::getLightType() const
{
    return static_cast<Lighting::LightType>(ui->lightTypeComboBox->currentData().toInt());
}

Lighting::LightMode LightEditWidget::getLightMode() const
{
    return static_cast<Lighting::LightMode>(ui->lightModeComboBox->currentData().toInt());
}

void LightEditWidget::onLightSettingChanged()
{
    emit sigUpdateLightInfo(getLightType(), getLightMode());
}

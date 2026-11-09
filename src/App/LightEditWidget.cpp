#include "LightEditWidget.h"
#include "ui_LightEditWidget.h"
#include "../Common/Translator.h"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSignalBlocker>
#include <QGridLayout>

LightEditWidget::LightEditWidget(QWidget *parent) : QWidget(parent), ui(new Ui::LightEditWidget)
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
    const char *labels[] = {"Light position X",  "Light position Y",  "Light position Z", "Light direction X",
                            "Light direction Y", "Light direction Z", "Inner angle",      "Outer angle"};
    for (int i = 0; i < 8; ++i)
    {
        ui->gridLayout_2->addWidget(new QLabel(TR(labels[i]), this), i + 2, 0);
        values_[i] = new QDoubleSpinBox(this);
        values_[i]->setDecimals(3);
        values_[i]->setRange(i < 6 ? -10000.0 : 0.0, i < 6 ? 10000.0 : 89.999);
        values_[i]->setSingleStep(i < 6 ? 0.1 : 1.0);
        ui->gridLayout_2->addWidget(values_[i], i + 2, 1);
    }
    error_ = new QLabel(this);
    error_->setWordWrap(true);
    error_->setStyleSheet("color: red");
    ui->gridLayout_2->addWidget(error_, 10, 0, 1, 2);
}

void LightEditWidget::initConnect()
{
    connect(ui->lightTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &LightEditWidget::onLightSettingChanged);
    connect(ui->lightModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &LightEditWidget::onLightSettingChanged);
    for (auto *value : values_)
    {
        connect(value, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                [this](double) { onValuesChanged(); });
    }
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

void LightEditWidget::setValues(Lighting::LightType type, Lighting::LightMode mode, const glm::vec3 &position,
                                const glm::vec3 &direction, float inner, float outer)
{
    const QSignalBlocker typeBlock(ui->lightTypeComboBox), modeBlock(ui->lightModeComboBox);
    ui->lightTypeComboBox->setCurrentIndex(ui->lightTypeComboBox->findData(static_cast<int>(type)));
    ui->lightModeComboBox->setCurrentIndex(ui->lightModeComboBox->findData(static_cast<int>(mode)));
    const float v[] = {position.x, position.y, position.z, direction.x, direction.y, direction.z, inner, outer};
    for (int i = 0; i < 8; ++i)
    {
        const QSignalBlocker block(values_[i]);
        values_[i]->setValue(v[i]);
        values_[i]->setEnabled(i < 3   ? type != Lighting::LightType::Directional
                               : i < 6 ? type != Lighting::LightType::Point
                                       : type == Lighting::LightType::Spot);
    }
    error_->clear();
}

void LightEditWidget::onValuesChanged()
{
    glm::vec3 position(values_[0]->value(), values_[1]->value(), values_[2]->value());
    glm::vec3 direction(values_[3]->value(), values_[4]->value(), values_[5]->value());
    const float inner = float(values_[6]->value()), outer = float(values_[7]->value());
    if (glm::dot(direction, direction) < 1e-8f || inner >= outer || outer >= 90.0f)
    {
        error_->setText(TR("Direction must be nonzero and 0 <= inner < outer < 90 degrees"));
        return;
    }
    error_->clear();
    emit sigUpdateLightValues(position, direction, inner, outer);
}

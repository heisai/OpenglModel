#include "LightEditWidget.h"
#include "ui_LightEditWidget.h"
#include "../Common/Translator.h"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSignalBlocker>
#include <QGridLayout>

// 创建面板并用完整默认参数初始化，注册结构体信号所需的元类型。
LightEditWidget::LightEditWidget(QWidget *parent) : QWidget(parent), ui(new Ui::LightEditWidget)
{
    ui->setupUi(this);
    qRegisterMetaType<Lighting::LightPanelData>();
    initUI();
    setPanelData(panel_data_);
    initConnect();
}

// 释放生成的 UI 对象。
LightEditWidget::~LightEditWidget()
{
    delete ui;
}

// 创建类型、模式、数值编辑控件及错误提示。
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

// 将所有控件变化汇总为同一个完整参数提交入口。
void LightEditWidget::initConnect()
{
    connect(ui->lightTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &LightEditWidget::onPanelChanged);
    connect(ui->lightModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &LightEditWidget::onPanelChanged);
    for (int i = 0; i < 8; ++i)
    {
        connect(values_[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                [this, i](double value) { onValueChanged(i, value); });
    }
}

// 返回含未舍入字段的编辑草稿，而非重新采样控件显示值。
Lighting::LightPanelData LightEditWidget::panelData() const
{
    return panel_data_;
}

// 更新类型和模式，并提交整份草稿供引擎校验。
void LightEditWidget::onPanelChanged()
{
    panel_data_.type = static_cast<Lighting::LightType>(ui->lightTypeComboBox->currentData().toInt());
    panel_data_.mode = static_cast<Lighting::LightMode>(ui->lightModeComboBox->currentData().toInt());
    emit sigUpdateLightPanel(panel_data_);
}

// 回显引擎已接受的快照，阻断信号以免产生递归提交。
void LightEditWidget::setPanelData(const Lighting::LightPanelData &data)
{
    panel_data_ = data;
    const QSignalBlocker typeBlock(ui->lightTypeComboBox), modeBlock(ui->lightModeComboBox);
    ui->lightTypeComboBox->setCurrentIndex(ui->lightTypeComboBox->findData(static_cast<int>(data.type)));
    ui->lightModeComboBox->setCurrentIndex(ui->lightModeComboBox->findData(static_cast<int>(data.mode)));
    const float values[] = {data.position.x,  data.position.y,  data.position.z,        data.direction.x,
                            data.direction.y, data.direction.z, data.innerAngleDegrees, data.outerAngleDegrees};
    for (int i = 0; i < 8; ++i)
    {
        const QSignalBlocker block(values_[i]);
        values_[i]->setValue(values[i]);
        values_[i]->setEnabled(i < 3   ? data.type != Lighting::LightType::Directional
                               : i < 6 ? data.type != Lighting::LightType::Point
                                       : data.type == Lighting::LightType::Spot);
    }
    error_->clear();
}

// 显示引擎拒绝草稿的原因，保留输入以便修正。
void LightEditWidget::showValidationError(const QString &message)
{
    error_->setText(message);
}

// 仅更新用户实际修改的字段，保留其他字段的完整精度。
void LightEditWidget::onValueChanged(int index, double value)
{
    if (index < 3)
    {
        panel_data_.position[index] = static_cast<float>(value);
    }
    else if (index < 6)
    {
        panel_data_.direction[index - 3] = static_cast<float>(value);
    }
    else if (index == 6)
    {
        panel_data_.innerAngleDegrees = static_cast<float>(value);
    }
    else
    {
        panel_data_.outerAngleDegrees = static_cast<float>(value);
    }
    onPanelChanged();
}

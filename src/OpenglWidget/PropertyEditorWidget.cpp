#include "PropertyEditorWidget.h"

PropertyEditorWidget::PropertyEditorWidget(QWidget *parent)
	: QWidget(parent),
	material_attrib_(std::make_unique<Utils::MaterialAttrib>())
{
	ui.setupUi(this);
	initUI();
	initConnect();
}



void PropertyEditorWidget::initUI()
{
	// 添加材质（key 需与 Translator / Utils::MaterialAttrib 中保持一致）

	// --- 宝石类（Gemstones） ---
	ui.MaterialComBox->addItem(TR("emerald"));
	ui.MaterialComBox->addItem(TR("jade"));
	ui.MaterialComBox->addItem(TR("obsidian"));
	ui.MaterialComBox->addItem(TR("pearl"));
	ui.MaterialComBox->addItem(TR("ruby"));
	ui.MaterialComBox->addItem(TR("turquoise"));

	// --- 金属类（Metals） ---
	ui.MaterialComBox->addItem(TR("brass"));
	ui.MaterialComBox->addItem(TR("bronze"));
	ui.MaterialComBox->addItem(TR("chrome"));
	ui.MaterialComBox->addItem(TR("copper"));
	ui.MaterialComBox->addItem(TR("gold"));
	ui.MaterialComBox->addItem(TR("silver"));

	// --- 塑料类（Plastics） ---
	ui.MaterialComBox->addItem(TR("black_plastic"));
	ui.MaterialComBox->addItem(TR("cyan_plastic"));
	ui.MaterialComBox->addItem(TR("green_plastic"));
	ui.MaterialComBox->addItem(TR("red_plastic"));
	ui.MaterialComBox->addItem(TR("white_plastic"));
	ui.MaterialComBox->addItem(TR("yellow_plastic"));

	// --- 橡胶类（Rubbers） ---
	ui.MaterialComBox->addItem(TR("black_rubber"));
	ui.MaterialComBox->addItem(TR("cyan_rubber"));
	ui.MaterialComBox->addItem(TR("green_rubber"));
	ui.MaterialComBox->addItem(TR("red_rubber"));
	ui.MaterialComBox->addItem(TR("white_rubber"));
	ui.MaterialComBox->addItem(TR("yellow_rubber"));
}

void PropertyEditorWidget::initConnect()
{
	connect(ui.MaterialComBox, &QComboBox::currentTextChanged, this, [this](const QString& text) {
		const Utils::Material value = material_attrib_->getMaterial(text);
		qDebug() <<text;
		emit sigMaterialChanged(value);
	});
}

	
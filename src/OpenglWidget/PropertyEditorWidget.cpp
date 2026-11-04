#include "PropertyEditorWidget.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

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

		// 设置默认光照模型
	ui.LightModelCombox->addItem(TR("phone"));	//phone 光照模型
	ui.LightModelCombox->addItem(TR("blinn"));		//blinn 光照模型


	// ����Ĭ�Ϲ���ģ��
	ui.RenderCombox->addItem(TR("double_render"));	//˫���ڹ�
	ui.RenderCombox->addItem(TR("single_render"));		//�����ڹ�
	ui.RenderCombox->addItem(TR("discard_render"));		//�Ƴ���Ⱦ

	auto* diffuseTextureLabel = new QLabel(tr("漫反射贴图"), this);
	diffuse_texture_path_edit_ = new QLineEdit(this);
	diffuse_texture_path_edit_->setReadOnly(true);
	diffuse_texture_browse_button_ = new QPushButton(tr("选择..."), this);
	diffuse_texture_clear_button_ = new QPushButton(tr("清除"), this);
	auto* diffuseTextureLayout = new QHBoxLayout();
	diffuseTextureLayout->addWidget(diffuse_texture_path_edit_);
	diffuseTextureLayout->addWidget(diffuse_texture_browse_button_);
	diffuseTextureLayout->addWidget(diffuse_texture_clear_button_);
	ui.gridLayout->addWidget(diffuseTextureLabel, 13, 0);
	ui.gridLayout->addLayout(diffuseTextureLayout, 13, 1);

	auto* specularTextureLabel = new QLabel(tr("镜面贴图"), this);
	specular_texture_path_edit_ = new QLineEdit(this);
	specular_texture_path_edit_->setReadOnly(true);
	specular_texture_browse_button_ = new QPushButton(tr("选择..."), this);
	specular_texture_clear_button_ = new QPushButton(tr("清除"), this);
	auto* specularTextureLayout = new QHBoxLayout();
	specularTextureLayout->addWidget(specular_texture_path_edit_);
	specularTextureLayout->addWidget(specular_texture_browse_button_);
	specularTextureLayout->addWidget(specular_texture_clear_button_);
	ui.gridLayout->addWidget(specularTextureLabel, 14, 0);
	ui.gridLayout->addLayout(specularTextureLayout, 14, 1);
}

void PropertyEditorWidget::initConnect()
{
	//更新材质属性
	connect(ui.MaterialComBox, &QComboBox::currentTextChanged, this, [this](const QString& text) {
		Utils::Material material_value = material_attrib_->getMaterial(text);
		material_value.light_model_type_ = ui.LightModelCombox->currentIndex();
		material_value.render_type_ = ui.RenderCombox->currentIndex();
		material_value.diffuse_texture_path_ = diffuse_texture_path_;
		material_value.specular_texture_path_ = specular_texture_path_;
		emit sigUpdatePropertyInfo(material_value);
	});
	//更新光照模型属性
	connect(ui.LightModelCombox, &QComboBox::currentIndexChanged, this, [this](const int &value) {
		QString text = ui.MaterialComBox->currentText();
		 Utils::Material material_value = material_attrib_->getMaterial(text);
		 material_value.light_model_type_ = value;
		 material_value.render_type_ = ui.RenderCombox->currentIndex();
		 material_value.diffuse_texture_path_ = diffuse_texture_path_;
		 material_value.specular_texture_path_ = specular_texture_path_;
		emit sigUpdatePropertyInfo(material_value);
		});

	//更新光照模型属性
	connect(ui.RenderCombox, &QComboBox::currentIndexChanged, this, [this](const int& value) {
		Utils::Material material_value = getPropertyInfo();
		material_value.light_model_type_ = ui.LightModelCombox->currentIndex();
		material_value.render_type_ = value;
		emit sigUpdatePropertyInfo(material_value);
		});

	connect(diffuse_texture_browse_button_, &QPushButton::clicked, this, [this]() {
		const QString path = QFileDialog::getOpenFileName(
			this, tr("Select diffuse texture"), QString(),
			tr("Image files (*.png *.jpg *.jpeg *.bmp *.tga *.gif);;All files (*)"));
		if (path.isEmpty()) return;
		diffuse_texture_path_ = std::filesystem::path(path.toStdWString());
		diffuse_texture_path_edit_->setText(path);
		emit sigUpdatePropertyInfo(getPropertyInfo());
	});
	connect(diffuse_texture_clear_button_, &QPushButton::clicked, this, [this]() {
		diffuse_texture_path_.reset();
		diffuse_texture_path_edit_->clear();
		emit sigUpdatePropertyInfo(getPropertyInfo());
	});
	connect(specular_texture_browse_button_, &QPushButton::clicked, this, [this]() {
		const QString path = QFileDialog::getOpenFileName(
			this, tr("Select specular texture"), QString(),
			tr("Image files (*.png *.jpg *.jpeg *.bmp *.tga *.gif);;All files (*)"));
		if (path.isEmpty()) return;
		specular_texture_path_ = std::filesystem::path(path.toStdWString());
		specular_texture_path_edit_->setText(path);
		emit sigUpdatePropertyInfo(getPropertyInfo());
	});
	connect(specular_texture_clear_button_, &QPushButton::clicked, this, [this]() {
		specular_texture_path_.reset();
		specular_texture_path_edit_->clear();
		emit sigUpdatePropertyInfo(getPropertyInfo());
	});
}

Utils::Material PropertyEditorWidget::getPropertyInfo()
{
	QString text = ui.MaterialComBox->currentText();
	Utils::Material material_value = material_attrib_->getMaterial(text);
	material_value.light_model_type_ = ui.LightModelCombox->currentIndex();
	material_value.render_type_ = ui.RenderCombox->currentIndex();
	material_value.diffuse_texture_path_ = diffuse_texture_path_;
	material_value.specular_texture_path_ = specular_texture_path_;
	return material_value;
}

	
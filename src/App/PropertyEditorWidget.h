#pragma once

#include <QWidget>
#include <memory>
#include <optional>
#include <filesystem>
#include "ui_PropertyEditorWidget.h"
#include"../Common/Translator.h"
#include"../Common/Utils.h"
#include<QDebug>

class QLineEdit;
class QPushButton;

class PropertyEditorWidget : public QWidget
{
    Q_OBJECT

public:
    PropertyEditorWidget(QWidget *parent = nullptr);
    ~PropertyEditorWidget() = default;

    void initUI();
    void initConnect();
    Utils::Material getPropertyInfo();
signals:
    void sigUpdatePropertyInfo(const Utils::Material& value);

private:
    Ui::PropertyEditorWidgetClass ui;
    std::unique_ptr<Utils::MaterialAttrib> material_attrib_;
    std::optional<std::filesystem::path> diffuse_texture_path_;
    std::optional<std::filesystem::path> specular_texture_path_;
    QLineEdit* diffuse_texture_path_edit_ = nullptr;
    QLineEdit* specular_texture_path_edit_ = nullptr;
    QPushButton* diffuse_texture_browse_button_ = nullptr;
    QPushButton* diffuse_texture_clear_button_ = nullptr;
    QPushButton* specular_texture_browse_button_ = nullptr;
    QPushButton* specular_texture_clear_button_ = nullptr;
};


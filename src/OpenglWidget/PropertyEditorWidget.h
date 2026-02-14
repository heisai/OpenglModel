#pragma once

#include <QWidget>
#include <memory>
#include "ui_PropertyEditorWidget.h"
#include"../Utils/Translator.h"
#include"../Utils/Utils.h"
#include<QDebug>
class PropertyEditorWidget : public QWidget
{
    Q_OBJECT

public:
    PropertyEditorWidget(QWidget *parent = nullptr);
    ~PropertyEditorWidget() = default;

    void initUI();
    void initConnect();
signals:
    void sigUpdatePropertyInfo(const Utils::Material& value);

private:
    Ui::PropertyEditorWidgetClass ui;
    std::unique_ptr<Utils::MaterialAttrib> material_attrib_;
};


#pragma once

#include <QWidget>
#include "ui_PropertyEditorWidget.h"

class PropertyEditorWidget : public QWidget
{
	Q_OBJECT

public:
	PropertyEditorWidget(QWidget *parent = nullptr);
	~PropertyEditorWidget();

private:
	Ui::PropertyEditorWidgetClass ui;
};


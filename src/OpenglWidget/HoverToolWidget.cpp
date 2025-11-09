
#include "HoverToolWidget.h"


HoverToolWidget::HoverToolWidget(QWidget* parent)
	: QWidget(parent)
	, m_layout(new QVBoxLayout(this))
{
	initializeWidget();
	for (QPushButton* button : buttons_)
	{
		connect(button, &QPushButton::clicked, this, &HoverToolWidget::clieckButton,Qt::UniqueConnection);
	}
}



void HoverToolWidget::initializeWidget()
{
	
	m_layout->setContentsMargins(5, 5, 5, 5);
	m_layout->setSpacing(6);
	buttons_.emplace_back(new QPushButton("Move", this));
	buttons_.emplace_back(new QPushButton("Rotate", this));
	buttons_.emplace_back(new QPushButton("Scale", this));
	buttons_.emplace_back(new QPushButton("Select", this));
	int index = 0;
	for (QPushButton* button : buttons_)
	{
		button->setFixedSize(QSize(60, 35));
		button->setObjectName(QString::number(index));
		m_layout->addWidget(button);
		index++;
	}
	space_item_ = new QSpacerItem(0, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);
	m_layout->addSpacerItem(space_item_);
	setLayout(m_layout);
	setFixedSize(QSize(70, 170));
	move(5, 5);
	setWindowFlags(Qt::FramelessWindowHint);  // ÎÞ±ß¿ò
	setAttribute(Qt::WA_OpaquePaintEvent, true);  // È·±£±³¾°Í¸Ã÷
	setAttribute(Qt::WA_TranslucentBackground);  // ÆôÓÃÍ¸Ã÷±³¾°
}

HoverToolWidget::ModelState HoverToolWidget::getModelState() const
{
	return static_cast<ModelState>(model_state_);
}

void HoverToolWidget::clieckButton(bool clieck)
{
	model_state_ = sender()->objectName().toUInt();
	for (auto &button : buttons_)
	{
		if (button->objectName() == sender()->objectName())
		{
			button->setStyleSheet("QPushButton {"
				"    background-color: #C0C0C0;"  
				"    color: Black;"
				"    border-radius: 4px;"
				"}"
				"QPushButton:hover {"
				"    background-color: #F5F5F5;"  
				"}"
				"QPushButton:pressed {"
				"    background-color: #D3D3D3;"  
				"}");
		}
		else
		{
			button->setStyleSheet("QPushButton {"
				"    background-color: #FFFFFF;"  
				"    color: Black;"
				"    border-radius: 4px;"
				"}"
				"QPushButton:hover {"
				"    background-color: #F5F5F5;"  
				"}"
				"QPushButton:pressed {"
				"    background-color: #D3D3D3;"  
				"}");
		}
	}
}

void HoverToolWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);  // ¿ªÆô¿¹¾â³Ý
	painter.setPen(Qt::NoPen);  // ²»»æÖÆ±ß¿ò
	painter.setBrush(QColor(169, 169, 169, 10));  // ÉèÖÃ°ëÍ¸Ã÷°×É«±³¾°
	painter.drawRoundedRect(this->rect(), 6, 6);  // »æÖÆÔ²½Ç¾ØÐÎ±³¾°
	QWidget::paintEvent(event);
}

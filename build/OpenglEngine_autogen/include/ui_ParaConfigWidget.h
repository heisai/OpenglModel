/********************************************************************************
** Form generated from reading UI file 'ParaConfigWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARACONFIGWIDGET_H
#define UI_PARACONFIGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ParaConfigWidget
{
public:
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBoxGrid;
    QCheckBox *checkBoxRect;
    QCheckBox *checkBoxModel;
    QCheckBox *checkBoxMix;
    QCheckBox *checkBoxTours;
    QCheckBox *checkBoxCylinder;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ParaConfigWidget)
    {
        if (ParaConfigWidget->objectName().isEmpty())
            ParaConfigWidget->setObjectName(QString::fromUtf8("ParaConfigWidget"));
        ParaConfigWidget->resize(250, 620);
        ParaConfigWidget->setMaximumSize(QSize(250, 16777215));
        verticalLayout_6 = new QVBoxLayout(ParaConfigWidget);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        groupBox = new QGroupBox(ParaConfigWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        checkBoxGrid = new QCheckBox(groupBox);
        checkBoxGrid->setObjectName(QString::fromUtf8("checkBoxGrid"));

        verticalLayout->addWidget(checkBoxGrid);

        checkBoxRect = new QCheckBox(groupBox);
        checkBoxRect->setObjectName(QString::fromUtf8("checkBoxRect"));

        verticalLayout->addWidget(checkBoxRect);

        checkBoxModel = new QCheckBox(groupBox);
        checkBoxModel->setObjectName(QString::fromUtf8("checkBoxModel"));

        verticalLayout->addWidget(checkBoxModel);

        checkBoxMix = new QCheckBox(groupBox);
        checkBoxMix->setObjectName(QString::fromUtf8("checkBoxMix"));

        verticalLayout->addWidget(checkBoxMix);

        checkBoxTours = new QCheckBox(groupBox);
        checkBoxTours->setObjectName(QString::fromUtf8("checkBoxTours"));

        verticalLayout->addWidget(checkBoxTours);

        checkBoxCylinder = new QCheckBox(groupBox);
        checkBoxCylinder->setObjectName(QString::fromUtf8("checkBoxCylinder"));

        verticalLayout->addWidget(checkBoxCylinder);


        verticalLayout_4->addLayout(verticalLayout);


        verticalLayout_5->addWidget(groupBox);


        verticalLayout_6->addLayout(verticalLayout_5);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer);


        retranslateUi(ParaConfigWidget);

        QMetaObject::connectSlotsByName(ParaConfigWidget);
    } // setupUi

    void retranslateUi(QWidget *ParaConfigWidget)
    {
        ParaConfigWidget->setWindowTitle(QCoreApplication::translate("ParaConfigWidget", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ParaConfigWidget", "\345\274\225\346\223\216", nullptr));
        checkBoxGrid->setText(QCoreApplication::translate("ParaConfigWidget", "\347\275\221\346\240\274", nullptr));
        checkBoxRect->setText(QCoreApplication::translate("ParaConfigWidget", "\347\253\213\346\226\271\344\275\223", nullptr));
        checkBoxModel->setText(QCoreApplication::translate("ParaConfigWidget", "\346\250\241\345\236\213", nullptr));
        checkBoxMix->setText(QCoreApplication::translate("ParaConfigWidget", "\346\267\267\345\220\210", nullptr));
        checkBoxTours->setText(QCoreApplication::translate("ParaConfigWidget", "\345\234\206\347\216\257", nullptr));
        checkBoxCylinder->setText(QCoreApplication::translate("ParaConfigWidget", "\345\234\206\346\237\261", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ParaConfigWidget: public Ui_ParaConfigWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARACONFIGWIDGET_H

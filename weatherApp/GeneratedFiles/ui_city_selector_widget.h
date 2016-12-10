/********************************************************************************
** Form generated from reading UI file 'city_selector_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CITY_SELECTOR_WIDGET_H
#define UI_CITY_SELECTOR_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_city_selector_widget
{
public:
    QGridLayout *gridLayout;
    QComboBox *cityCombo;
    QLabel *label;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *city_selector_widget)
    {
        if (city_selector_widget->objectName().isEmpty())
            city_selector_widget->setObjectName(QStringLiteral("city_selector_widget"));
        city_selector_widget->resize(400, 300);
        gridLayout = new QGridLayout(city_selector_widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        cityCombo = new QComboBox(city_selector_widget);
        cityCombo->setObjectName(QStringLiteral("cityCombo"));

        gridLayout->addWidget(cityCombo, 0, 1, 1, 1);

        label = new QLabel(city_selector_widget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);


        retranslateUi(city_selector_widget);

        QMetaObject::connectSlotsByName(city_selector_widget);
    } // setupUi

    void retranslateUi(QWidget *city_selector_widget)
    {
        city_selector_widget->setWindowTitle(QApplication::translate("city_selector_widget", "city_selector_widget", 0));
        label->setText(QApplication::translate("city_selector_widget", "Please Select City", 0));
    } // retranslateUi

};

namespace Ui {
    class city_selector_widget: public Ui_city_selector_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CITY_SELECTOR_WIDGET_H

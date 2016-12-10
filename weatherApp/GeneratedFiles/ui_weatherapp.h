/********************************************************************************
** Form generated from reading UI file 'weatherapp.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEATHERAPP_H
#define UI_WEATHERAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "city_selector_widget.hpp"

QT_BEGIN_NAMESPACE

class Ui_weatherappClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLabel *testLabel;
    QPushButton *testButton;
    QLabel *tempLabel;
    QVBoxLayout *graphicsLayout;
    CitySelectorWidget *widget;
    QMenuBar *menuBar;
    QMenu *menuHello;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *weatherappClass)
    {
        if (weatherappClass->objectName().isEmpty())
            weatherappClass->setObjectName(QStringLiteral("weatherappClass"));
        weatherappClass->resize(670, 534);
        centralWidget = new QWidget(weatherappClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        testLabel = new QLabel(centralWidget);
        testLabel->setObjectName(QStringLiteral("testLabel"));

        gridLayout->addWidget(testLabel, 1, 0, 1, 1);

        testButton = new QPushButton(centralWidget);
        testButton->setObjectName(QStringLiteral("testButton"));

        gridLayout->addWidget(testButton, 3, 0, 1, 1);

        tempLabel = new QLabel(centralWidget);
        tempLabel->setObjectName(QStringLiteral("tempLabel"));

        gridLayout->addWidget(tempLabel, 2, 0, 1, 1);

        graphicsLayout = new QVBoxLayout();
        graphicsLayout->setSpacing(6);
        graphicsLayout->setObjectName(QStringLiteral("graphicsLayout"));

        gridLayout->addLayout(graphicsLayout, 4, 0, 1, 1);

        widget = new CitySelectorWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));

        gridLayout->addWidget(widget, 0, 0, 1, 1);

        weatherappClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(weatherappClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 670, 38));
        menuHello = new QMenu(menuBar);
        menuHello->setObjectName(QStringLiteral("menuHello"));
        weatherappClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(weatherappClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        weatherappClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(weatherappClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        weatherappClass->setStatusBar(statusBar);

        menuBar->addAction(menuHello->menuAction());

        retranslateUi(weatherappClass);

        QMetaObject::connectSlotsByName(weatherappClass);
    } // setupUi

    void retranslateUi(QMainWindow *weatherappClass)
    {
        weatherappClass->setWindowTitle(QApplication::translate("weatherappClass", "weatherapp", 0));
        testLabel->setText(QApplication::translate("weatherappClass", "Test Label", 0));
        testButton->setText(QApplication::translate("weatherappClass", "PushButton", 0));
        tempLabel->setText(QApplication::translate("weatherappClass", "TextLabel", 0));
        menuHello->setTitle(QApplication::translate("weatherappClass", "Hello?", 0));
    } // retranslateUi

};

namespace Ui {
    class weatherappClass: public Ui_weatherappClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEATHERAPP_H

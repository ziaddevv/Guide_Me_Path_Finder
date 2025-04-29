/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *firstCityLbl;
    QLabel *secondcityLbl;
    QComboBox *FirstCityCmb;
    QComboBox *secondCityCmb;
    QPushButton *bfsBtn;
    QPushButton *dfsBtn;
    QPushButton *dijkstraBtn;
    QLineEdit *resultText;
    QLineEdit *chosenCities;
    QLineEdit *resultText2;
    QComboBox *StartcityCmb;
    QLabel *startCityLbl;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1120, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        firstCityLbl = new QLabel(centralwidget);
        firstCityLbl->setObjectName("firstCityLbl");
        firstCityLbl->setGeometry(QRect(50, 40, 71, 24));
        secondcityLbl = new QLabel(centralwidget);
        secondcityLbl->setObjectName("secondcityLbl");
        secondcityLbl->setGeometry(QRect(380, 40, 111, 24));
        FirstCityCmb = new QComboBox(centralwidget);
        FirstCityCmb->setObjectName("FirstCityCmb");
        FirstCityCmb->setGeometry(QRect(180, 40, 141, 32));
        secondCityCmb = new QComboBox(centralwidget);
        secondCityCmb->setObjectName("secondCityCmb");
        secondCityCmb->setGeometry(QRect(520, 40, 131, 32));
        bfsBtn = new QPushButton(centralwidget);
        bfsBtn->setObjectName("bfsBtn");
        bfsBtn->setGeometry(QRect(950, 330, 100, 33));
        dfsBtn = new QPushButton(centralwidget);
        dfsBtn->setObjectName("dfsBtn");
        dfsBtn->setGeometry(QRect(950, 380, 100, 33));
        dijkstraBtn = new QPushButton(centralwidget);
        dijkstraBtn->setObjectName("dijkstraBtn");
        dijkstraBtn->setGeometry(QRect(950, 120, 100, 33));
        resultText = new QLineEdit(centralwidget);
        resultText->setObjectName("resultText");
        resultText->setGeometry(QRect(130, 130, 761, 32));
        chosenCities = new QLineEdit(centralwidget);
        chosenCities->setObjectName("chosenCities");
        chosenCities->setGeometry(QRect(240, 90, 331, 32));
        resultText2 = new QLineEdit(centralwidget);
        resultText2->setObjectName("resultText2");
        resultText2->setGeometry(QRect(130, 350, 771, 32));
        StartcityCmb = new QComboBox(centralwidget);
        StartcityCmb->setObjectName("StartcityCmb");
        StartcityCmb->setGeometry(QRect(230, 270, 141, 32));
        startCityLbl = new QLabel(centralwidget);
        startCityLbl->setObjectName("startCityLbl");
        startCityLbl->setGeometry(QRect(60, 270, 161, 24));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1120, 29));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        firstCityLbl->setText(QCoreApplication::translate("MainWindow", "First City", nullptr));
        secondcityLbl->setText(QCoreApplication::translate("MainWindow", "Second City", nullptr));
        bfsBtn->setText(QCoreApplication::translate("MainWindow", "BFS", nullptr));
        dfsBtn->setText(QCoreApplication::translate("MainWindow", "DFS", nullptr));
        dijkstraBtn->setText(QCoreApplication::translate("MainWindow", "Dijkstra", nullptr));
        startCityLbl->setText(QCoreApplication::translate("MainWindow", "choose Start City", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

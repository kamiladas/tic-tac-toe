/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *hostButton;
    QPushButton *findButton;
    QLineEdit *serverIpLineEdit;
    QPushButton *connectButton;
    QLabel *hostIpLabel;
    QPushButton *button00;
    QPushButton *button01;
    QPushButton *button02;
    QPushButton *button10;
    QPushButton *button11;
    QPushButton *button12;
    QPushButton *button20;
    QPushButton *button21;
    QPushButton *button22;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setGeometry(QRect(0, 0, 400, 300));
        hostButton = new QPushButton(centralwidget);
        hostButton->setObjectName(QString::fromUtf8("hostButton"));
        hostButton->setGeometry(QRect(10, 10, 75, 23));
        findButton = new QPushButton(centralwidget);
        findButton->setObjectName(QString::fromUtf8("findButton"));
        findButton->setGeometry(QRect(90, 10, 75, 23));
        serverIpLineEdit = new QLineEdit(centralwidget);
        serverIpLineEdit->setObjectName(QString::fromUtf8("serverIpLineEdit"));
        serverIpLineEdit->setGeometry(QRect(10, 220, 200, 30));
        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setGeometry(QRect(220, 220, 100, 30));
        hostIpLabel = new QLabel(centralwidget);
        hostIpLabel->setObjectName(QString::fromUtf8("hostIpLabel"));
        hostIpLabel->setGeometry(QRect(10, 190, 200, 20));
        button00 = new QPushButton(centralwidget);
        button00->setObjectName(QString::fromUtf8("button00"));
        button00->setGeometry(QRect(50, 50, 75, 75));
        button01 = new QPushButton(centralwidget);
        button01->setObjectName(QString::fromUtf8("button01"));
        button01->setGeometry(QRect(130, 50, 75, 75));
        button02 = new QPushButton(centralwidget);
        button02->setObjectName(QString::fromUtf8("button02"));
        button02->setGeometry(QRect(210, 50, 75, 75));
        button10 = new QPushButton(centralwidget);
        button10->setObjectName(QString::fromUtf8("button10"));
        button10->setGeometry(QRect(50, 130, 75, 75));
        button11 = new QPushButton(centralwidget);
        button11->setObjectName(QString::fromUtf8("button11"));
        button11->setGeometry(QRect(130, 130, 75, 75));
        button12 = new QPushButton(centralwidget);
        button12->setObjectName(QString::fromUtf8("button12"));
        button12->setGeometry(QRect(210, 130, 75, 75));
        button20 = new QPushButton(centralwidget);
        button20->setObjectName(QString::fromUtf8("button20"));
        button20->setGeometry(QRect(50, 210, 75, 75));
        button21 = new QPushButton(centralwidget);
        button21->setObjectName(QString::fromUtf8("button21"));
        button21->setGeometry(QRect(130, 210, 75, 75));
        button22 = new QPushButton(centralwidget);
        button22->setObjectName(QString::fromUtf8("button22"));
        button22->setGeometry(QRect(210, 210, 75, 75));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Tic Tac Toe", nullptr));
        hostButton->setText(QCoreApplication::translate("MainWindow", "Host", nullptr));
        findButton->setText(QCoreApplication::translate("MainWindow", "Find Game", nullptr));
        serverIpLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter server IP", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        hostIpLabel->setText(QCoreApplication::translate("MainWindow", "Host IP: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

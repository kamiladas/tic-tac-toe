/********************************************************************************
** Form generated from reading UI file 'findgamedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDGAMEDIALOG_H
#define UI_FINDGAMEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FindGameDialog
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *hostListWidget;
    QPushButton *connectButton;

    void setupUi(QDialog *FindGameDialog)
    {
        if (FindGameDialog->objectName().isEmpty())
            FindGameDialog->setObjectName(QString::fromUtf8("FindGameDialog"));
        FindGameDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(FindGameDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        hostListWidget = new QListWidget(FindGameDialog);
        hostListWidget->setObjectName(QString::fromUtf8("hostListWidget"));

        verticalLayout->addWidget(hostListWidget);

        connectButton = new QPushButton(FindGameDialog);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        verticalLayout->addWidget(connectButton);


        retranslateUi(FindGameDialog);

        QMetaObject::connectSlotsByName(FindGameDialog);
    } // setupUi

    void retranslateUi(QDialog *FindGameDialog)
    {
        FindGameDialog->setWindowTitle(QCoreApplication::translate("FindGameDialog", "Find Game", nullptr));
        connectButton->setText(QCoreApplication::translate("FindGameDialog", "Connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindGameDialog: public Ui_FindGameDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDGAMEDIALOG_H

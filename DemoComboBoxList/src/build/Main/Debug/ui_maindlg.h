/********************************************************************************
** Form generated from reading UI file 'maindlg.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINDLG_H
#define UI_MAINDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_MainDlg
{
public:
    QComboBox *comboBoxColor;
    QComboBox *comboBoxDB;
    QComboBox *comboBox_3;
    QComboBox *comboBox_4;

    void setupUi(QDialog *MainDlg)
    {
        if (MainDlg->objectName().isEmpty())
            MainDlg->setObjectName(QStringLiteral("MainDlg"));
        MainDlg->resize(402, 300);
        comboBoxColor = new QComboBox(MainDlg);
        comboBoxColor->setObjectName(QStringLiteral("comboBoxColor"));
        comboBoxColor->setGeometry(QRect(110, 41, 171, 31));
        comboBoxDB = new QComboBox(MainDlg);
        comboBoxDB->setObjectName(QStringLiteral("comboBoxDB"));
        comboBoxDB->setGeometry(QRect(110, 100, 171, 31));
        comboBox_3 = new QComboBox(MainDlg);
        comboBox_3->setObjectName(QStringLiteral("comboBox_3"));
        comboBox_3->setGeometry(QRect(110, 160, 171, 31));
        comboBox_4 = new QComboBox(MainDlg);
        comboBox_4->setObjectName(QStringLiteral("comboBox_4"));
        comboBox_4->setGeometry(QRect(110, 220, 171, 31));

        retranslateUi(MainDlg);

        QMetaObject::connectSlotsByName(MainDlg);
    } // setupUi

    void retranslateUi(QDialog *MainDlg)
    {
        MainDlg->setWindowTitle(QApplication::translate("MainDlg", "MainDlg", 0));
    } // retranslateUi

};

namespace Ui {
    class MainDlg: public Ui_MainDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDLG_H

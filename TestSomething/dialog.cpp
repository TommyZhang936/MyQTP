#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include "chinesenumber.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //initSome();

    QWidget* pParent = (QWidget*)QApplication::desktop();
    cn1 = new ChineseNumber(pParent);
    cn1->show();

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::initSome()
{
    int coded = 0;
    int year = 2016;
    int month = 1;
    for(; month <12; month++)
    {
            int day = 1;
            for(; day <31; day++)
            {
                coded = year + month * 100 + day;
                qDebug() << "Coded is : " <<coded;
            }
    }
    
    int codet = 0;
    int hour = 23;
    int minutes = 0;
    for(; minutes <59; minutes++)
    {
            int second = 0;
            for(; second <31; second++)
            {
                codet = (hour + minutes) * 100 + second;
                qDebug() << "Codet is : " <<codet;
            }
    }

}



void Dialog::on_pushButton_clicked()
{
    cn1->setStart(0);
}

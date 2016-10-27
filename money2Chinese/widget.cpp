#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
   newMHZ1 = new MoneyToHZ;
   
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        return;
    }
    
    QString str = newMHZ1->getHZ(arg1.toDouble());
    ui->textBrowser->setText(str);
}

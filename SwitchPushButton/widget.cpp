#include "widget.h"
#include "ui_widget.h"
#include <QHBoxLayout>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    
    newSB1 = new SwitchButton;
    newSB2 = new SwitchButton;
    
    QHBoxLayout *thisLayput = new QHBoxLayout(this);
    thisLayput->addWidget(newSB1);
    thisLayput->addWidget(newSB2);
    this->setLayout(thisLayput);
    
    connect(newSB1, SIGNAL(clicked(bool)), this, SLOT(ShowStatus()));
    connect(newSB2, SIGNAL(clicked(bool)), this, SLOT(ShowStatus()));
    
}

Widget::~Widget()
{
    delete ui;
}

void Widget::ShowStatus()
{
    strText1 = (newSB1->isON()) ? "1 - ON" : "1 - OFF";
    strText2 = (newSB2->isON()) ? "2 - ON" : "2 - OFF";
    
    ui->label_1->setText(strText1);
    ui->label_2->setText(strText2);
}

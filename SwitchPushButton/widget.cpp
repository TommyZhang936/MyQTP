#include "widget.h"
#include "ui_widget.h"
#include <QHBoxLayout>
#include <QPropertyAnimation>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    
    newSB1 = new SwitchButton;
    newSB2 = new SwitchButton;
    
    imageButton = new ImageButton;
    QString imag1 = QString(":/1.png");
    QString imagh = QString(":/1_hover.png");
    QString imagp = QString(":/1_pressed.png");
    imageButton->setImages(imag1, imagh , imagp);
    imageButton->setFixedSize(QSize(128, 128));
    
    imageBok = new ImageButton;
    QString imag2 = QString(":/btn_ok.png");
    QString imag2h = QString(":/btn_ok_hover.png");
    QString imag2p = QString(":/btn_ok_pressed.png");
    imageBok->setImages(imag2, imag2h , imag2p);
    imageBok->setFixedSize(QSize(64, 64));
    
    QHBoxLayout *thisLayput = new QHBoxLayout;
    thisLayput->addWidget(newSB1);
    thisLayput->addWidget(newSB2);
    thisLayput->addWidget(imageButton);
    thisLayput->addWidget(imageBok);
    
    maskLab = new MaskLabel;
    maskLab->setPixmap(QPixmap(":/LABEL.png"));
    //maskLab->SetRadius(100);
    QHBoxLayout *thisLout = new QHBoxLayout;
    thisLout->addWidget(maskLab);
    
    ui->widget_2->setLayout(thisLout);    
    ui->widget_1->setLayout(thisLayput);
    
    connect(newSB1, SIGNAL(clicked(bool)), this, SLOT(ShowStatus()));
    connect(newSB2, SIGNAL(clicked(bool)), this, SLOT(ShowStatus()));
    connect(imageBok, SIGNAL(clicked(bool)), this, SLOT(ShowAM()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::ShowStatus()
{
    strText = (newSB1->isON()) ? "1 - ON" : "1 - OFF";
    strText += "     ";
    strText += (newSB2->isON()) ? "2 - ON" : "2 - OFF";
    strText += QString(" Radius: %1").arg(maskLab->radius);
    ui->label_1->setText(strText);
}

void Widget::ShowAM()
{    
    QPropertyAnimation *pAnimation = new QPropertyAnimation(maskLab, "radius");
    pAnimation->setDuration(5000);
    pAnimation->setKeyValueAt(0, 0);
    pAnimation->setKeyValueAt(0.2, 300);
    pAnimation->setKeyValueAt(1, 0);
    pAnimation->start();
//    int nowR = maskLab->radius;
//    if(nowR < 300)
//    {
//        nowR++;
//        maskLab->SetRadius(nowR);
//        //maskLab->update();
//    }
}

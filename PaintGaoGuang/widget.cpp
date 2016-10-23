#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QGraphicsEffect>
#include <QLinearGradient>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);    
    
    //构建阴影
    //QGraphicsEffect的4个子类QGraphicsBlurEffect模糊, QGraphicsColorizeEffect变色, QGraphicsDropShadowEffect阴影, 和 QGraphicsOpacityEffect透明
    //1、模糊
    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
    effect->setBlurRadius(1.5);     //模糊度，值越大越模糊
    //2、阴影
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setColor(QColor(200, 0, 0));          //阴影的颜色
    shadowEffect->setBlurRadius(8);                           //模糊度
//    shadowEffect->setXOffset(5);                               //方向
//    shadowEffect->setYOffset(5);
    shadowEffect->setOffset(8, 8);                                 //这句直接指定2个方向
    //3、变色
    QGraphicsColorizeEffect *colorChange = new QGraphicsColorizeEffect(this);
    colorChange->setColor(QColor(0, 192, 192));                  //着色颜色
    colorChange->setStrength(180);                               //着色强度
    //4、透明
    QGraphicsOpacityEffect *e4 = new QGraphicsOpacityEffect(this);
    e4->setOpacity(0.8);                  //透明值取值0 - 1.0之间
    QRect rect = this->rect();
    QLinearGradient alphaGradient(rect.topLeft(), rect.bottomLeft());
    alphaGradient.setColorAt(0.0, Qt::transparent);
    alphaGradient.setColorAt(0.5, Qt::black);
    alphaGradient.setColorAt(1.0, Qt::transparent);
    e4->setOpacityMask(alphaGradient);           //设置遮罩
        
    //应用   
    ui->checkBox->setGraphicsEffect(effect);
    ui->pushButton->setGraphicsEffect(shadowEffect);
    ui->toolButton->setGraphicsEffect(colorChange);
    ui->radioButton->setGraphicsEffect(e4);
}

Widget::~Widget()
{
    delete ui;
}

//高光按钮
void Widget::paintEvent(QPaintEvent *) 
{
    QPainter painter(this);

    painter.translate(width() / 2, height() / 2);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::transparent);
    painter.fillRect(-width(), -height(), width() * 2, height() * 2, Qt::gray);

    int radius = (width() > height()) ? height() / 2 : width() / 2;
    if(radius > 33)
        radius -= 33;
    //int radius = 100;
    ui->pushButton->setText(QString("%1").arg(radius));

    // 外边框
    QLinearGradient lg1(0, -radius, 0, radius);
    lg1.setColorAt(0, QColor(255, 255, 255));
    lg1.setColorAt(1, QColor(166, 166, 166));
    painter.setBrush(lg1);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    double inzi = radius / 100;
    // 内边框
    radius -= (13 * inzi);
    QLinearGradient lg2(0, -radius, 0, radius);
    lg2.setColorAt(0, QColor(155, 155, 155));
    lg2.setColorAt(1, QColor(255, 255, 255));
    painter.setBrush(lg2);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    // 内部的圆
    radius -= (4 * inzi);
    QRadialGradient rg(0, 0, radius);
    rg.setColorAt(0, QColor(245, 0, 0));
    rg.setColorAt(0.6, QColor(210, 0, 0));
    rg.setColorAt(1, QColor(140, 0, 0));
    painter.setBrush(rg);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    // 高光
    radius -= 3;
    QPainterPath path;
    path.addEllipse(-radius, -radius - 2, radius << 1, radius << 1);

    QPainterPath bigEllipse;
    radius *= 2;
    ui->checkBox->setText(QString("Radius: %1").arg(radius));
    bigEllipse.addEllipse(-radius, -radius + 140 * inzi, radius << 1, radius << 1);

    path -= bigEllipse;

    QLinearGradient lg3(0, -radius / 2, 0, 0);
    lg3.setColorAt(0, QColor(255, 255, 255, 220));
    lg3.setColorAt(1, QColor(255, 255, 255, 30));
    painter.setBrush(lg3);
    painter.drawPath(path);
}

#include "widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}

Widget::~Widget()
{
    
}

void Widget::paintEvent(QPaintEvent *event)  
{  
    QPainter painter1(this);  //新建画笔  
    painter1.setRenderHint(QPainter::Antialiasing, true);  //设置 反走样  
    painter1.setPen(QPen(Qt::black, 8, Qt::DashDotLine, Qt::RoundCap));  //设置线条 颜色， 大小，线型风格，拐点风格  
    painter1.setBrush(QBrush(Qt::green, Qt::SolidPattern));  //设置画刷 颜色 和 风格  
    painter1.drawEllipse(10, 10, 160, 80);   //画椭圆  
  
    QPainter painter2(this);  
    painter2.setRenderHint(QPainter::Antialiasing, true);  
    painter2.setPen(QPen(Qt::black, 8, Qt::DashDotLine, Qt::RoundCap));  
    painter2.setBrush(QBrush(Qt::blue, Qt::DiagCrossPattern));  
    painter2.drawPie(10, 150, 200, 120, 60*16, 270*16);  
  
    QPainter painter3(this);  
    painter3.setRenderHint(QPainter::Antialiasing, true);  
    QPainterPath path;    //轨迹  
    path.moveTo(200, 150);   //先移到这个点  
    path.cubicTo(250, 80, 320, 80, 400, 150); //x1 x2 和 andpoint 确定弧形  
    painter3.setPen(QPen(Qt::black, 8));  
    painter3.setBrush(QBrush(Qt::green, Qt::DiagCrossPattern));  
  
    QTransform transform;      //世界变换  
    transform.translate(0, 0);  //以00为中心 选装30°  
    transform.rotate(+30.0);  
    painter3.setWorldTransform(transform);   //添加世界变换  
    painter3.drawPath(path);  
}  

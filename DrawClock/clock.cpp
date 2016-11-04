#include "Clock.h"  
#include <QPainter>  
#include <QTime>  
  
QPoint sec[4] = {QPoint(0, 10), QPoint(3, 0), QPoint(0, -90), QPoint(-3, 0)};  
QPoint min[4] = {QPoint(0, 8), QPoint(5, 0), QPoint(0, -70), QPoint(-5, 0)};  
QPoint hour[4] = {QPoint(0, 6), QPoint(7, 0), QPoint(0, -50), QPoint(-7, 0)};  
  
Clock::Clock(QWidget *parent):QWidget(parent)  
{  
    setMinimumSize(QSize(200, 200));
    resize(200, 200);
    //setWindowFlags(Qt::FramelessWindowHint);   
    
    timer = new QTimer(this);  
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));          //update()会自动产生重绘消息，调用paintEvent()  
    timer->start(1000);  
}  
  
Clock::~Clock()  
{  

}  
  
void Clock::paintEvent(QPaintEvent *)  
{  
    QPainter painter(this);  
    painter.setRenderHint(QPainter::Antialiasing);  
    painter.translate(width() >> 1, height() >> 1);  
    //painter.setWindow(-100, -100, 200, 200);  
  
    //绘制大刻度  
    painter.save();                               //save() 与 restore() 必须是一一对应的  
    for (int i = 0; i < 60 ; ++i)  
    {  
        painter.rotate(6);                      //rotate()是顺时针旋转坐标轴  
        if (i % 5 == 4)  
        {  
            painter.setPen(QPen(Qt::black, 3));  
            painter.drawLine(0, -90, 0, -100);  
        }  
        else  
        {  
            painter.setPen(QPen(Qt::black, 1));  
            painter.drawLine(0, -95, 0, -100);  
        }  
    }  
    painter.restore();  
    
    int s = QTime::currentTime().second();  
    int m = QTime::currentTime().minute();  
    int h = QTime::currentTime().hour();  
    //绘制文字时间
    painter.drawText(-20, 50, QTime::currentTime().toString());  
    //绘制 时针
    painter.save();  
    painter.rotate(((h % 12) +  static_cast<double>(m / 60.0)) * 30);  
    painter.setBrush(Qt::blue);  
    painter.setPen(Qt::blue);  
    painter.drawConvexPolygon(hour, 4);  
    painter.restore();  
    //绘制 分针
    painter.save();
    QRadialGradient radialGradientM(QPointF(0, 8), 70);  
    radialGradientM.setColorAt(0, QColor(0, 250, 0, 150));  
    radialGradientM.setColorAt(1, QColor(0, 20, 0, 50));  
    painter.setBrush(radialGradientM);
    painter.setPen(Qt::NoPen); 
    painter.drawConvexPolygon(min, 4);  
    painter.restore();  
    //绘制秒针
    painter.save();  
    painter.rotate(s * 6);  
    QRadialGradient radialGradient(QPointF(0, 10), 100);  
    radialGradient.setColorAt(0, QColor(255, 0, 0, 150));  
    radialGradient.setColorAt(1, QColor(20, 0, 0, 50));  
    painter.setBrush(radialGradient);
    painter.setPen(Qt::NoPen);  
    painter.drawConvexPolygon(sec, 4);  
    painter.restore();  
}  

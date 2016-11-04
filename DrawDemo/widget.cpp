#include "widget.h"
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    resize(800, 600);
    
    QTimer *timer = new QTimer(this);  
    connect(timer, SIGNAL(timeout()), this, SLOT(newPoint()));          //update()会自动产生重绘消息，调用paintEvent()  
    timer->start(1000); 
    
}

Widget::~Widget()
{
    
}

void Widget::paintEvent(QPaintEvent *)  
{
    QPainter painter(this);  
    painter.setRenderHint(QPainter::Antialiasing);  
    //写坐标
    int fontSize = 20;
    QFont font("Arial", fontSize, QFont::Bold, true);
    painter.setFont(font);
    painter.setPen(QColor(rand() % 256, rand() % 256, rand() % 256));
    painter.drawText(QRect(0, height() - (fontSize << 1), width(), fontSize << 1), Qt::AlignCenter, QStringLiteral("坐标：(%1,%2)").arg(point.x()).arg(point.y()));
    
    //切换原点到屏幕中心      
    painter.setPen(Qt::red);
    painter.drawLine(QPoint(width() >> 1, 0), QPoint(width() >> 1, height()));
    painter.drawLine(QPoint(0, height() >> 1), QPoint(width(), height() >> 1));
    
    huaDian(painter);
    huaXian(painter);


}

void Widget::newPoint()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int x = rand() % (width() >> 1);
    int y = rand() % (height() >> 1);
    x = (rand() % 2) ? -x : x;
    y = (rand() % 2) ? -y : y;
    qDebug() << x << y;
    pointPre = point;
    point = QPoint(x, y);

    update();
}

void Widget::huaDian(QPainter &painter)
{
    // 0-画点
    painter.save();
    painter.translate(width() >> 1, height() >> 1);   
    painter.setPen(QColor(rand() % 256, rand() % 256, rand() % 256));
    painter.drawEllipse(point, 10, 10);
    painter.restore();
}

void Widget::huaXian(QPainter &painter)
{
    QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
    linearGrad.setColorAt(0, QColor(rand() % 256, rand() % 256, rand() % 256));
    linearGrad.setColorAt(1, QColor(rand() % 256, rand() % 256, rand() % 256));
    
    QRadialGradient radialGrad(QPointF(0, 0), 200);
    radialGrad.setColorAt(0, QColor(rand() % 256, rand() % 256, rand() % 256));
    radialGrad.setColorAt(0.5, QColor(rand() % 256, rand() % 256, rand() % 256));
    radialGrad.setColorAt(1, QColor(rand() % 256, rand() % 256, rand() % 256));
    
    QPen pen;
    pen.setWidth(5);
    //pen.setBrush(linearGrad);
    pen.setBrush(radialGrad);
    // 1-画线
    painter.save();
    painter.translate(width() >> 1, height() >> 1);   
    painter.setPen(pen);
    painter.drawLine(pointPre, point);    
    painter.restore();
}

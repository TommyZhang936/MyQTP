#include "widget.h"
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    resize(1024, 768);
    
    QTimer *timer = new QTimer(this);  
    connect(timer, SIGNAL(timeout()), this, SLOT(newPoint()));          //update()会自动产生重绘消息，调用paintEvent()  
    timer->start(1000); 
    myPath = new QPainterPath;
    
}

Widget::~Widget()
{
    
}

void Widget::paintEvent(QPaintEvent *)  
{
    QPainter painter(this);  
    painter.setRenderHint(QPainter::Antialiasing);  
    
    //painter.setViewport(0, 0, this->width(), this->height());
    //painter.setWindow(QRect(-800, -600, 800, 600));

    huaYuandian(painter);
    huaDian(painter);
    huaXian(painter);

    //自定义带箭头线段    
    DrawCustomLine(painter, 0, 150, "Da", Qt::red);  
    DrawCustomLine(painter, 123.5, 130, "Db", Qt::green);  
    DrawCustomLine(painter, -45, 30, "Dc", Qt::blue);  
    DrawCustomLine(painter, -145, 60, "Dd", Qt::yellow);  
        
    huaPath(painter);
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
    painter.save();
    // 0-画点
    painter.translate(width() >> 1, height() >> 1);   
    painter.drawEllipse(QRect(-150, -150, 300, 300));  
    painter.setPen(QColor(rand() % 256, rand() % 256, rand() % 256));
    painter.drawEllipse(point, 10, 10);
    painter.restore();
}

void Widget::huaXian(QPainter &painter)
{
    painter.save();
    
    QLinearGradient linearGrad(pointPre, point);
    linearGrad.setColorAt(0, QColor(rand() % 256, rand() % 256, rand() % 256));
    linearGrad.setColorAt(1, QColor(rand() % 256, rand() % 256, rand() % 256));
    
    QRadialGradient radialGrad(QPointF(0, 0), 200);
    radialGrad.setColorAt(0, QColor(rand() % 256, rand() % 256, rand() % 256));
    radialGrad.setColorAt(0.5, QColor(rand() % 256, rand() % 256, rand() % 256));
    radialGrad.setColorAt(1, QColor(rand() % 256, rand() % 256, rand() % 256));
    
    QPen pen;
    pen.setWidth(5);
    pen.setBrush(linearGrad);
    //pen.setBrush(radialGrad);
    // 1-画线
    painter.translate(width() >> 1, height() >> 1);   
    painter.setPen(pen);
    painter.drawLine(pointPre, point);    
    
    painter.restore();
}

void Widget::huaYuandian(QPainter &painter)
{
    painter.save();
    //写坐标原点
    int fontSize = 20;
    QFont font("Arial", fontSize, QFont::Bold, true);
    painter.setFont(font);
    painter.setPen(QColor(rand() % 256, rand() % 256, rand() % 256));
    painter.drawText(QRect(0, height() - (fontSize << 1), width(), fontSize << 1), Qt::AlignCenter, QStringLiteral("坐标：(%1,%2)").arg(point.x()).arg(point.y()));

    
    //切换原点到屏幕中心      
    painter.setPen(Qt::red);    
    painter.drawLine(QPoint(width() >> 1, 0), QPoint(width() >> 1, height()));
    painter.drawLine(QPoint(0, height() >> 1), QPoint(width(), height() >> 1));

    painter.restore();
}
//画带箭头的线
void Widget::DrawCustomLine(QPainter & _painter, double _angle, double _len, const QString & _name, const QColor & _color)  
{  
    _painter.save();
    // 初始化  
    _painter.resetTransform(); // 重设画笔  
    //_painter.translate(QPoint(300,300));  
    _painter.translate(width() >> 1, height() >> 1);   
    _painter.setPen(QPen(Qt::black, 2));  
  
    // 绘制线段  
    QLine _line(QPoint(0, 0), QPoint(_len, 0));  
    _painter.rotate(_angle);  
    _painter.drawLine(_line);  
  
    // 绘制箭头  
    int off_x = 10;  
    QPainterPath _path;  
    _path.moveTo(_len - off_x, -5);  
    _path.lineTo(_len, 0);  
    _path.lineTo(_len - off_x, 5);  
    _painter.drawPath(_path);  
  
    // 绘制名称  
    _painter.setPen(_color);  
      
    QFont _font = _painter.font();  
    _font.setPointSize(10);  
    _font.setBold(true);  
    _painter.setFont(_font);  
  
    _painter.drawText(QPoint(_len - 2 * off_x, -10), _name);  
    
    _painter.restore();
}  

//写路径
void Widget::huaPath(QPainter &painter)
{
    painter.save();
    
    QLinearGradient linearGrad(QPointF(10, 0), QPointF(600, 0));
    linearGrad.setColorAt(0,Qt::black);
    linearGrad.setColorAt(0.5, Qt::red);
    linearGrad.setColorAt(1,Qt::blue);
    QFont font("微软雅黑", 80);
    font.setBold(true);
    QPainterPath textPath;
    textPath.addText(10, 100, font, QStringLiteral("电子工业出版社"));
    painter.setBrush(linearGrad);
    painter.setPen(Qt::NoPen);
    painter.drawPath(textPath);
    
    painter.restore();
}

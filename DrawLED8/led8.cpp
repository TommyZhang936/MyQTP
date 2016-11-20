#include "led8.h"
#include <QPainter>

LEDO::LEDO(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    wLED = 10.0f;
    hLED = wLED * 4;
    whLED = hLED * 0.7;
    resize(wLED * 6 + whLED, wLED * 9 + hLED * 2);

}

LEDO::~LEDO()
{
}

void LEDO::paintEvent(QPaintEvent *)
{
    //绘制函数,没什么好说的
    QPainter p(this);
    //p.fillRect(this->rect(), Qt::black);
    //p.fillRect(this->rect(), QBrush(img));
    //QPainter::RenderHint的可取如表6-1中的值
    //------------------------------------
    //QPainter::Antialiasing           告诉绘图引擎应该在可能的情况下进行边的反锯齿绘制
    //QPainter::TextAntialiasing       尽可能的情况下文字的反锯齿绘制
    //QPainter::SmoothPixmapTransform  使用平滑的pixmap变换算法(双线性插值算法),而不是近邻插值算法
    p.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, true);
    p.save();
    //-----------------
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::red);

    QPointF op = QPointF(0, 0);
    drawLED8(p, op);

    p.restore();
}

void LEDO::drawLED8(QPainter &p, QPointF op)
{
    QPointF pointO = op;
    //左边竖行
    QPointF point0 = pointO + QPointF(wLED, wLED + wLED);
    calcHPoints(point0);
    p.drawPolygon(mPoints);
    point0 = pointO + QPointF(wLED, wLED * 4 + wLED + hLED);
    calcHPoints(point0);
    p.drawPolygon(mPoints);
    //右边竖行
    point0 = pointO + QPointF(wLED * 5 + whLED, wLED + wLED);
    calcHPoints(point0);
    p.drawPolygon(mPoints);
    point0 = pointO + QPointF(wLED * 5 + whLED, wLED * 5 + hLED);
    calcHPoints(point0);
    p.drawPolygon(mPoints);
    //横画
    point0 = pointO + QPointF(wLED + wLED, wLED);
    calcWPoints(point0);
    p.drawPolygon(mPoints);
    point0 = pointO + QPointF(wLED + wLED, wLED * 4.5 + hLED);
    calcWPoints(point0);
    p.drawPolygon(mPoints);
    point0 = pointO + QPointF(wLED + wLED, wLED * 8 + hLED * 2);
    calcWPoints(point0);
    p.drawPolygon(mPoints);
}

void LEDO::calcHPoints(QPointF sp)
{
    mPoints.clear();
    mPoints << sp
            << QPointF(sp + QPointF(-wLED, wLED))
            << QPointF(sp + QPointF(-wLED, wLED + hLED))
            << QPointF(sp + QPointF(0, wLED + wLED + hLED))
            << QPointF(sp + QPointF(wLED, wLED + hLED))
            << QPointF(sp + QPointF(wLED, wLED));
}

void LEDO::calcWPoints(QPointF sp)
{
    mPoints.clear();
    mPoints << sp
            << QPointF(sp + QPointF(wLED, wLED))
            << QPointF(sp + QPointF(wLED + whLED, wLED))
            << QPointF(sp + QPointF(wLED + whLED + wLED, 0))
            << QPointF(sp + QPointF(wLED + whLED, -wLED))
            << QPointF(sp + QPointF(wLED, -wLED));
}

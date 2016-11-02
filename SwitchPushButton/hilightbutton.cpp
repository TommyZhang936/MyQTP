#include "hilightbutton.h"
#include <QPainter>
#include <QDebug>

HiLightButton::HiLightButton(QWidget *parent)
    : QWidget(parent),
      radius(100)
{
    setWindowFlags(Qt::FramelessWindowHint);
}

void HiLightButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.translate(width() / 2, height() / 2);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::transparent);

    int runRadius = radius;

    // 外边框
    QLinearGradient lg1(0, -runRadius, 0, runRadius);
    lg1.setColorAt(0, QColor(255, 255, 255));
    lg1.setColorAt(1, QColor(166, 166, 166));
    painter.setBrush(lg1);
    painter.drawEllipse(-runRadius, -runRadius, runRadius << 1, runRadius << 1);

    // 内边框
    runRadius -= (runRadius / 10);
    QLinearGradient lg2(0, -runRadius, 0, runRadius);
    lg2.setColorAt(0, QColor(155, 155, 155));
    lg2.setColorAt(1, QColor(255, 255, 255));
    painter.setBrush(lg2);
    painter.drawEllipse(-runRadius, -runRadius, runRadius << 1, runRadius << 1);

    // 内部的圆
    runRadius -= (runRadius / 15);
    QRadialGradient rg(0, 0, runRadius);
    rg.setColorAt(0, QColor(245, 0, 0));
    rg.setColorAt(0.6, QColor(210, 0, 0));
    rg.setColorAt(1, QColor(140, 0, 0));
    painter.setBrush(rg);
    painter.drawEllipse(-runRadius, -runRadius, runRadius << 1, runRadius << 1);

    // 高光
    runRadius -= (runRadius / 28);
    QPainterPath path;
    path.addEllipse(-runRadius, -runRadius - 2, runRadius << 1, runRadius << 1);

    runRadius *= 2;
    QPainterPath bigEllipse;
    //bigEllipse.addEllipse(-runRadius, -runRadius + 140, runRadius << 1, runRadius << 1);
    bigEllipse.addEllipse(-runRadius, -16, runRadius << 1, runRadius << 1);
    path -= bigEllipse;

    qDebug() << "Here : " << -runRadius + 140;
    QLinearGradient lg3(0, -runRadius / 2, 0, 0);
    lg3.setColorAt(0, QColor(255, 255, 255, 220));
    lg3.setColorAt(1, QColor(255, 255, 255, 30));
    painter.setBrush(lg3);

    painter.drawPath(path);
}

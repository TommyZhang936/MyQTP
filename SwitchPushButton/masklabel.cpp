#include "masklabel.h"
#include <QPainter>
#include <QDebug>

MaskLabel::MaskLabel(QWidget *parent) 
    : QLabel(parent),
      radius(50)
{
}

void MaskLabel::paintEvent(QPaintEvent *e)
{
    if(pixmap())
    {
        int picX = ((pixmap()->width()) >> 1) - radius;
        int picY = ((pixmap()->height()) >> 1) - radius;
   
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        QPainterPath path;
        path.addEllipse(picX, picY, radius << 1, radius << 1);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, width(), height(), *pixmap());
    }
    else
    {
        QLabel::paintEvent(e);
    }
}

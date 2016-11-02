#include "rotarybutton.h"
#include <QPainter>
#include <QMatrix>
#include <QDebug>

RotaryButton::RotaryButton(QWidget *parent)
    : QWidget(parent),
      angle(0),
      pixWidth(85),
      pixHeight(85)             //列表初始化按照定义先后顺序进行
{   
    qreal stx = - pixWidth >> 1;
    qreal sty = - pixHeight >> 1;
    rectPix = QRect(stx, sty, pixWidth, pixHeight);
    
    enterAnimation = new QPropertyAnimation(this, "angle");
    enterAnimation->setStartValue(0);
    enterAnimation->setEndValue(90);
	enterAnimation->setDuration(200);
    
    leaveAnimation = new QPropertyAnimation(this, "angle");
    leaveAnimation->setStartValue(90);
    leaveAnimation->setEndValue(0);
	leaveAnimation->setDuration(200);
    
}

RotaryButton::~RotaryButton()
{
    delete enterAnimation;
	delete leaveAnimation;
}

void RotaryButton::setImage(QString image)
{
	this->image = image;
    
	update();
}

void RotaryButton::setText(QString text)
{
	this->text = text;
	update();
}

void RotaryButton::enterEvent(QEvent *)
{
    enterAnimation->start();
}

void RotaryButton::leaveEvent(QEvent *)
{
    leaveAnimation->start();
}

/*
void RotaryButton::paintEvent(QPaintEvent *)
{
	if (image.isEmpty()) 
    {
		return;
	}

    QPainter painter(this);
    painter.save();
	painter.setRenderHint(QPainter::Antialiasing);

    //定图片圆心
    qreal pixX = rect().center().x() - pixWidth / 2;
    qreal pixY = rect().center().y() - pixHeight / 2 - 10;
    QPoint point(pixX, pixY);

    //图片旋转角度-angle
    //旋转过程中图片会变小
//    double pi = 3.14;    
//    double a    = pi / 180 * angle;
//    double sina = sin(a);
//    double cosa = cos(a);
    
//    QMatrix translationMatrix(1.0, 0, 0, -1.0, -0.5, 0.5);
//    QMatrix rotationMatrix(cosa, sina, -sina, cosa, 0, 0);
//    QMatrix scalingMatrix(1.0, 0, 0, -1.0, 0.5, 0.5);
    
//    QMatrix matrix;
//    matrix =  scalingMatrix * rotationMatrix * translationMatrix;
  
    QMatrix matrix;
    matrix.reset();
    matrix.rotate(angle);    
    qDebug() << matrix.m11() << matrix.m12();
    QImage img(image);
    img =  img.transformed(matrix, Qt::SmoothTransformation);
    QPixmap pix = QPixmap::fromImage(img);
    pix = pix.scaled(pixWidth, pixHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter.drawPixmap(point, pix);

    painter.restore();
    painter.drawText(QRectF(0, height() - 20, width(), 20), Qt::AlignCenter, text);
}
*/

void RotaryButton::paintEvent(QPaintEvent *)
{
	if (image.isEmpty()) 
    {
		return;
	}

    QPainter painter(this);
    painter.save();
    
	painter.setRenderHint(QPainter::Antialiasing, true);
    
    QPixmap pix(image);
    pix = pix.scaled(pixWidth, pixHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    
    // >> 1（右移1位）相当于width() / 2
    painter.translate(width() >> 1, height() >> 1);    
    // 旋转
    painter.rotate(angle);    
    
    painter.drawPixmap(rectPix, pix);

    painter.restore();
    painter.drawText(QRectF(0, height() - 20, width(), 20), Qt::AlignCenter, text);
    
}


#include "rotarybutton.h"
#include <QPainter>
#include <QMatrix>
#include <QDebug>

RotaryButton::RotaryButton(QWidget *parent)
    : QWidget(parent),
<<<<<<< HEAD
      angle(0),
      pixWidth(0),
      pixHeight(0)             //列表初始化按照定义先后顺序进行
{   
=======
      angle(0)
{
    pixWidth = 0;
	pixHeight = 0;
      
>>>>>>> 291ec0a3dd14e83069641ba3098ad848463efc3f
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
    QPixmap pix(image);
    
    pixWidth = pix.width() - 25;
	pixHeight = pix.height() - 25;
    
    qreal stx = - pixWidth >> 1;
    qreal sty = - pixHeight >> 1;
    rectPix = QRect(stx, sty, pixWidth, pixHeight);
    
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
<<<<<<< HEAD
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
=======

    QImage img(image);
    img = img.scaled(pixWidth, pixHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	painter.setRenderHint(QPainter::Antialiasing);
    
    QPixmap pix(image);
    int pixX = rect().center().x() - pixWidth / 2;
    int pixY = rect().center().y() - pixHeight / 2 - 10;
    QPoint point(pixX, pixY);

    QMatrix matrix;
    matrix.rotate(angle);
    img =  img.transformed(matrix, Qt::SmoothTransformation);
    pix = QPixmap::fromImage(img);
    pix = pix.scaled(pixWidth, pixHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

//    QTransform tf;
//    tf.translate(pix.size().width() / 2, pix.size().height() / 2);
//    tf.rotate(angle);
//    painter.setTransform(tf);

    //qDebug() << angle << point << pix.width() << pix.height() << pixWidth << pixHeight;
//    painter.translate(pixX, pixY); //让图片的中心作为旋转的中心
//    painter.rotate(angle); //顺时针旋转90度
//    painter.translate(-pixX, -pixY); //使原点复原
>>>>>>> 291ec0a3dd14e83069641ba3098ad848463efc3f

    painter.drawPixmap(point, pix);

    painter.restore();
    painter.drawText(QRectF(0, height() - 20, width(), 20), Qt::AlignCenter, text);
}
<<<<<<< HEAD
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

=======


>>>>>>> 291ec0a3dd14e83069641ba3098ad848463efc3f

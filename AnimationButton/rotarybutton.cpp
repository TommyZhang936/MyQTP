#include "rotarybutton.h"
#include <QPainter>
#include <QMatrix>
#include <QDebug>

RotaryButton::RotaryButton(QWidget *parent)
    : QWidget(parent),
      angle(0)
{
    pixWidth = 0;
	pixHeight = 0;
      
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

void RotaryButton::paintEvent(QPaintEvent *)
{
	if (image.isEmpty()) 
    {
		return;
	}

    QPainter painter(this);
    painter.save();

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

    painter.drawPixmap(point, pix);

    painter.restore();
    painter.drawText(QRectF(0, height() - 20, width(), 20), Qt::AlignCenter, text);
}



#include "rotarybutton.h"
#include <QPainter>
#include <QMatrix>
#include <QDebug>

RotaryButton::RotaryButton(QWidget *parent)
    : QWidget(parent),
      angle(0),
      pixWidth(86),
      pixHeight(86)             
    //列表初始化按照定义先后顺序进行
    //长宽需设置为双数，单数会导致后面旋转偏移
{   
    setLength();
    
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

void RotaryButton::setLength(int width, int height)
{
    pixWidth = width;
    pixHeight = height;
    qreal stx = - pixWidth >> 1;
    qreal sty = - pixHeight >> 1;
    rectPix = QRect(stx, sty, pixWidth, pixHeight);
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
	painter.setRenderHint(QPainter::Antialiasing, true);
    //终于搞完美了 经验：转换旋转一定要放在最前面*****
    // >> 1（右移1位）相当于width() / 2
    painter.translate(width() >> 1, (height() >> 1) - 10);    
    // 旋转
    painter.rotate(angle);   
    
    QPixmap pix(image);
    pix = pix.scaled(pixWidth, pixHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);    
    
    //绘制图标
    painter.drawPixmap(rectPix, pix);
    //恢复painter再绘制文字
    painter.restore();
    painter.drawText(QRectF(0, height() - 20, width(), 20), Qt::AlignCenter, text);
    
}


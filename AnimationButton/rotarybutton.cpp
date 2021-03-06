#include "rotarybutton.h"
#include <QPainter>
#include <QMatrix>
#include <QDebug>

RotaryButton::RotaryButton(QWidget *parent)
    : QWidget(parent),
      angle(0)          
    //列表初始化按照定义先后顺序进行
{   
    //初始化图标大小默认值
    setLength();
    
    rbAnimation = new QPropertyAnimation(this, "angle");
    rbAnimation->setStartValue(0);
    rbAnimation->setEndValue(90);
	rbAnimation->setDuration(200);
    
}

RotaryButton::~RotaryButton()
{
    delete rbAnimation;

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
//长宽需设置为双数，单数会导致后面旋转偏移
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
    setCursor(Qt::PointingHandCursor);
    rbAnimation->setDirection(QAbstractAnimation::Forward);
    rbAnimation->start();
}

void RotaryButton::leaveEvent(QEvent *)
{
    setCursor(Qt::ArrowCursor);
    rbAnimation->setDirection(QAbstractAnimation::Backward);
    rbAnimation->start();
}

void RotaryButton::paintEvent(QPaintEvent *)
{
	if (image.isEmpty()) 
    {
		return;
	}

    QPainter painter(this);
    painter.save();    
	//painter.setRenderHint(QPainter::Antialiasing, true);
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


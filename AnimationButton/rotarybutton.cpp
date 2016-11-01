#include "rotarybutton.h"
#include <QGraphicsScale>
#include <QGraphicsView>
#include <QPushButton>
#include <QTimer>
#include <QMatrix>

#include "synchapi.h"

RotaryButton::RotaryButton(QWidget *parent)
    : QWidget(parent)
{
    enter = false;
	leave = true;
    pixWidth = 0;
	pixHeight = 0;
    
    m_angle = 0;  
    m_delta = 0;  
    m_current = 0;  
    
    enterAnimation = new QPropertyAnimation(this, "");
	enterAnimation->setStartValue(1);
	enterAnimation->setEndValue(90);
	enterAnimation->setDuration(200);
	connect(enterAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(enterImageChanged(QVariant)));
    
    leaveAnimation = new QPropertyAnimation(this, "");
	leaveAnimation->setStartValue(1);
	leaveAnimation->setEndValue(90);
	leaveAnimation->setDuration(200);
	connect(leaveAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(leaveImageChanged(QVariant)));
    
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
	enter = true;
	leave = false;
    
    enterAnimation->start();
}

void RotaryButton::leaveEvent(QEvent *)
{
	enter = false;
	leave = true;
    
    leaveAnimation->start();
}

void RotaryButton::paintEvent(QPaintEvent *)
{
	if (image.isEmpty()) 
    {
		return;
	}

    QImage imgRatate;
    imgRatate.load(image);
         
    QPainter painter(this);
    painter.save();

	painter.setRenderHint(QPainter::Antialiasing);

	//QPixmap pix(image);
    
    int pixX = rect().center().x() - pixWidth / 2;
    int pixY = rect().center().y() - pixHeight / 2 - 10;
    QPoint point(pixX, pixY);
    
    QPixmap pix;    
	if (enter || leave) 
    {      
        QMatrix matrix;
        matrix.rotate(15);
        QImage img =  imgRatate.transformed(matrix, Qt::SmoothTransformation);
        pix = QPixmap::fromImage(img.scaled(pixWidth, pixHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));   
		//pix = QPixmap::fromImage(img);

	}       
    //pix.scaled(pixWidth, pixHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(point, pix); 
    
    painter.restore();
    painter.drawText(QRectF(0, height() - 20, width(), 20), Qt::AlignCenter, text);
}

void RotaryButton::enterImageChanged(QVariant index)
{
	int i = index.toInt();
	m_angle = i ;
	update();
}

void RotaryButton::leaveImageChanged(QVariant index)
{
	int i = index.toInt();
	m_angle = 90 - i;
	update();
}

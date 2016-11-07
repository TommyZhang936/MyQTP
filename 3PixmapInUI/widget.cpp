#include "widget.h"
#include <QMouseEvent>

Widget::Widget(QWidget *parent /* = 0 */)  
: QWidget(parent)  
{  
    m_MouseDown = false;  
    m_RectPen = new RectPen;  
    m_LinePen = new LinePen;  
    m_CirclePen = new CirclePen;  
    m_LineButton = new QRadioButton("Line", this);  
    m_RectButton = new QRadioButton("Rect", this);  
    m_CircleButton = new QRadioButton("Circle", this);  
    m_LineButton->move(10, 10);  
    m_RectButton->move(100, 10);  
    m_CircleButton->move(200, 10);  
    connect(m_LineButton, SIGNAL(clicked()), this, SLOT(ClickedLineButton()));  
    connect(m_RectButton, SIGNAL(clicked()), this, SLOT(ClickedRectButton()));  
    connect(m_CircleButton, SIGNAL(clicked()), this, SLOT(ClickedCircleButton()));  
    m_BasePen = m_LinePen;  
    m_LineButton->setChecked(true);  
    setFixedSize(500, 500);  
}  
  
Widget::~Widget()  
{  
//    delete m_LinePen;  
//    delete m_RectPen;  
//    delete m_CirclePen;  
}  
  
void Widget::mousePressEvent(QMouseEvent *event)  
{  
    if( event->button() == Qt::LeftButton )  
    {  
        m_MouseDown = true;  
        m_BasePen->SetStartPoint(event->pos());  
    }  
}  
  
void Widget::mouseMoveEvent(QMouseEvent *event)  
{  
    if( m_MouseDown )  
    {  
        m_BasePen->SetEndPoint(event->pos());  
        update();  
    }  
}  
  
void Widget::mouseReleaseEvent(QMouseEvent *event)  
{  
    if( event->button() == Qt::LeftButton )  
    {  
        m_MouseDown = false;  
    }  
}  
  
void Widget::paintEvent(QPaintEvent *event)  
{  
    Q_UNUSED(event);
    
    QPixmap temp = m_BasePen->GetPixmap();  
    QPainter painter(this);  
    painter.drawPixmap(0, 0, temp);  
}  


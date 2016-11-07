#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QRadioButton>
  
//状态类的基类，定义了各个公用接口，  
//其中，SetPixmap是一个纯虚接口  
class BasePen  
{  
protected:  
    //这三个类成员理应是BasePen的私有成员，然后通过接口访问  
    //我这里为了方便，直接把它们设为保护成员了  
    QPixmap m_Pixmap;  
    QPoint m_StartPoint;  
    QPoint m_EndPoint;  
    virtual void SetPixmap() = 0;  
    
public:  
    BasePen()  
    {   
        m_StartPoint = m_EndPoint = QPoint(0, 0);  
        m_Pixmap = QPixmap(500, 500);  
    }  
    void SetStartPoint(QPoint point) { m_StartPoint = point; }  
    void SetEndPoint(QPoint point)  
    {   
        m_EndPoint = point;   
        SetPixmap();  
    }  
    QPixmap GetPixmap() { return m_Pixmap; }  
};  
  
//矩形类，在界面上画一个红色的矩形  
class RectPen : public BasePen  
{  
protected:  
    void SetPixmap()  
    {  
        m_Pixmap.fill(Qt::white);  
        QPainter painter(&m_Pixmap);  
        QRect rect(m_StartPoint, m_EndPoint);  
        painter.setPen(Qt::red);  
        painter.drawRect(rect);  
    }  
};  
  
//直线类，在界面上画一条蓝色的直线  
class LinePen : public BasePen  
{  
protected:  
    void SetPixmap()  
    {  
        m_Pixmap.fill(Qt::white);  
        QPainter painter(&m_Pixmap);  
        painter.setPen(Qt::blue);  
        painter.drawLine(m_StartPoint, m_EndPoint);  
    }  
};  
  
//圆形类，在界面上画一个绿色的椭圆  
class CirclePen : public BasePen  
{  
protected:  
    void SetPixmap()  
    {  
        m_Pixmap.fill(Qt::white);  
        QPainter painter(&m_Pixmap);  
        QRect rect(m_StartPoint, m_EndPoint);  
        painter.setPen(Qt::green);  
        painter.drawEllipse(rect);  
    }  
};  
  
class Widget : public QWidget  
{  
    Q_OBJECT  
private:  
    bool m_MouseDown;  
    BasePen *m_BasePen;  
    RectPen *m_RectPen;  
    LinePen *m_LinePen;  
    CirclePen *m_CirclePen;  
    //在界面上放三个按钮，用来控制画图状态  
    QRadioButton *m_LineButton;  
    QRadioButton *m_RectButton;  
    QRadioButton *m_CircleButton;  
    
protected:  
    void mousePressEvent(QMouseEvent *event);  
    void mouseMoveEvent(QMouseEvent *event);  
    void mouseReleaseEvent(QMouseEvent *event);  
    void paintEvent(QPaintEvent *event);  
    
public:  
    Widget(QWidget *parent = 0);  
    ~Widget();  

private slots:  
        void ClickedLineButton() { m_BasePen = m_LinePen; }  
        void ClickedRectButton() { m_BasePen = m_RectPen; }  
        void ClickedCircleButton() { m_BasePen = m_CirclePen; }  
};  
  



#endif // WIDGET_H

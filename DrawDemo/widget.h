#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//class QPainter;

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    
    void huaYuandian(QPainter &painter);
    void huaDian(QPainter &);
    void huaXian(QPainter &);
    void DrawCustomLine(QPainter & _painter, double _angle, double _len, const QString & _name, const QColor & _color);
    void huaPath(QPainter &painter);
    
protected:
    void paintEvent(QPaintEvent *event);  
    

public slots:
    void newPoint();
    
private:
    QPoint point;
    QPoint pointPre;
    QPainterPath *myPath;
};

#endif // WIDGET_H

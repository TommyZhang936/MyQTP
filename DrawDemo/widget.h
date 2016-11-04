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
    
    void huaDian(QPainter &);
    void huaXian(QPainter &);
    
protected:
    void paintEvent(QPaintEvent *event);  
    

public slots:
    void newPoint();
    
private:
    QPoint point;
    QPoint pointPre;
};

#endif // WIDGET_H

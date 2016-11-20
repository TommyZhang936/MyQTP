#ifndef LED8H
#define LED8H

#include <QWidget>

class LEDO : public QWidget
{
    Q_OBJECT
    
public:
    explicit LEDO(QWidget *parent = 0);
    ~LEDO();
    
protected:
    void paintEvent(QPaintEvent *);
        
private:
    void calcHPoints(QPointF sp);
    void calcWPoints(QPointF sp);
    void drawLED8(QPainter &p, QPointF op);
    
private:
    float wLED;
    float hLED;
    float whLED;
    QPolygonF mPoints;

};

#endif // LED8H

#ifndef MASKLABEL_H
#define MASKLABEL_H

#include <QLabel>

class MaskLabel : public QLabel
{
    Q_OBJECT
    
    Q_PROPERTY(int radius READ getRadius WRITE setRadius)
    
public:
    explicit MaskLabel(QWidget *parent = 0);
    int getRadius() {return radius;}
    void setRadius(int m_radius)
    {
        radius = m_radius;
        update();             //半径改变后刷新显示
    }

protected:
    void paintEvent(QPaintEvent *);
    
public:
    int radius;

};

#endif // MASKLABEL_H

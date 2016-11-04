#ifndef CLOCK_H
#define CLOCK_H

#include <QWidget>
#include <QPoint>  
#include <QTimer>  

class Clock : public QWidget
{
    Q_OBJECT
    
public:
    explicit Clock(QWidget *parent = 0);
    ~Clock();
    
protected:  
    void paintEvent(QPaintEvent *event);  
    
private:  
    QTimer *timer; 

};

#endif // CLOCK_H

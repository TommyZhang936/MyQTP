#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    
private:
    void generateRandomNumber();
    void generateUniqueRandomNumber();
    void generateAscendRandomNumber();
};

#endif // WIDGET_H

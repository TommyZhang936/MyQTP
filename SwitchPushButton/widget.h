#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "switchbutton.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private slots:
    void ShowStatus();
    
private:
    Ui::Widget *ui;
    
    SwitchButton *newSB1;
    SwitchButton *newSB2;
    
    QString strText1;
    QString strText2;
};

#endif // WIDGET_H

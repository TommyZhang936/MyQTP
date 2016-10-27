#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "moneytohz.h"

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
    void on_lineEdit_textChanged(const QString &arg1);
    
private:
    Ui::Widget *ui;
    
    MoneyToHZ *newMHZ1;
};

#endif // WIDGET_H

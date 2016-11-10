#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "switchbutton.h"
#include "imagebutton.h"
#include "masklabel.h"
#include "switchbuttont.h"

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
    void ShowAM();
    void ShowRM();
    void setRadius();
    
private:
    Ui::Widget *ui;
    
    SwitchButton *newSB1;
    SwitchButton *newSB2;
    ImageButton *imageButton;
    ImageButton *imageBok;
    MaskLabel *maskLab;
    SwitchButtonT * newST1;
    
    QString strText;
};

#endif // WIDGET_H

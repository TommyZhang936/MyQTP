#ifndef SWITCHPUSHBUTTON_H
#define SWITCHPUSHBUTTON_H

//#include <QWidget>
#include <QPushButton>

class SwitchButton : public QPushButton
{
    Q_OBJECT
    
    Q_PROPERTY(bool isCheck READ isON WRITE SetON)
    
public:
    explicit SwitchButton(QWidget *parent = 0);
    ~SwitchButton();
    
    bool isON() const {return isCheck;}
    void SetON(bool sCheck);
   
private slots:
    void ChangeOnOff();

private:
    bool isCheck;
    QString styleOn;
    QString styleOff;
    
};

#endif // SWITCHPUSHBUTTON_H
